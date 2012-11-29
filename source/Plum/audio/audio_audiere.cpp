#include "../common/file.h"
#include "audio.h"

#include <audiere.h>

namespace plum
{
    class AudierePlumFile : public audiere::RefImplementation<audiere::File>
    {
        public:
            AudierePlumFile(plum::File* f) 
            {
                file = f;
            }

            virtual ~AudierePlumFile()
            {
                delete file;
            }

            virtual int ADR_CALL read(void* buffer, int size)
            {
                return file->readRaw(buffer, size);
            }

            virtual bool ADR_CALL seek(int position, audiere::File::SeekMode mode)
            {
                FileSeekMode m;
                switch (mode)
                {
                    case BEGIN: m = SeekStart; break;
                    case CURRENT: m = SeekCurrent; break;
                    case END: m = SeekEnd; break;
                    default: return false;
                }
                return file->seek(position, m);
            }

            virtual int ADR_CALL tell()
            {
                return file->tell();
            }

        private:
            plum::File* file;
    };

    class AudiereSound : public Sound
    {
        public:
            audiere::SampleBufferPtr buffer;
            AudiereSound(audiere::SampleBufferPtr buffer)
                : buffer(buffer)
            {
            }
    };

    class AudiereChannel : public Channel
    {
        public:
            audiere::OutputStreamPtr stream;
            bool shouldDispose;

            AudiereChannel(audiere::OutputStreamPtr channel)
            {
                this->stream = channel;
                shouldDispose = false;
            }

            void dispose()
            {
                shouldDispose = true;
            }

            void resume()
            {
                stream->play();
            }

            void pause()
            {
                stream->stop();
            }
       
            void stop()
            {
                stream->stop();
                stream->reset();
            }

            bool isPlaying() const
            {
                return stream->isPlaying();
            }

            double getVolume() const
            {
                return stream->getVolume();
            }

            double getPitch() const
            {
                return stream->getPitchShift();
            }

            void setVolume(double volume)
            {
                stream->setVolume((float) volume);
            }

            void setPitch(double pitch)
            {
                stream->setPitchShift((float) pitch);
            }
    };

    class AudiereSong : public Song
    {
        public:
            audiere::SampleSourcePtr sample;
            Channel* channel;
            double volume;

            AudiereSong() : sample(0), channel(0), volume(1.0)
            {
            }

            ~AudiereSong()
            {
                stop();
            }

            void resume()
            {
                if(channel)
                {
                    channel->resume();
                }
            }

            void pause()
            {
                if(channel)
                {
                    channel->pause();
                }
            }

            void stop()
            {
                if(channel)
                {
                    channel->stop();
                    channel->dispose();
                    channel = NULL;
                }
            }

            bool isPlaying() const
            {
                return channel && channel->isPlaying();
            }

            double getVolume() const
            {
                return this->volume;
            }

            double getPitch() const
            {
                return channel->getPitch();
            }

            void setVolume(double volume)
            {
                this->volume = volume;
                if(channel)
                {
                    channel->setVolume(volume);
                }
            }

            void setPitch(double pitch)
            {
                channel->setPitch(pitch);
            }
    };

    class AudiereAudio : public Audio
    {
        private:
            static const int STEPS_TO_CLEANUP = 50;
            int cleanupSteps;

            double masterPitch;

            bool disabled;
            audiere::AudioDevicePtr device;
            std::vector<audiere::OutputStreamPtr> soundStreamPool;
            std::vector<AudiereChannel*> activeChannels;

            int addSoundStream(audiere::OutputStreamPtr stream);
            Channel* loadChannel(audiere::OutputStreamPtr stream);

        public:
            AudiereAudio(audiere::AudioDevicePtr device, bool disabled);
            ~AudiereAudio();

            double getMasterPitch() const;
            void setMasterPitch(double pitch);

            Sound* loadSound(const std::string& filename);
            Song* loadSong(const std::string& filename);
            Channel* loadChannel(int handle);

            int playSound(Sound* sound, double volume = 1.0);
            void playSong(Song* song);

            void update();
    };

    Audio* Audio::create(bool disabled)
    {
        audiere::AudioDevicePtr device;
        if(!disabled)
        {
            device = audiere::OpenDevice();
        }
        if(!device)
        {
            device = audiere::OpenDevice("null", "");
        }
        return new AudiereAudio(device, disabled);
    }

    AudiereAudio::AudiereAudio(audiere::AudioDevicePtr device, bool disabled)
        : cleanupSteps(0), masterPitch(1.0),
        disabled(disabled), device(device)
    {
    }

    AudiereAudio::~AudiereAudio()
    {
        device = nullptr;
    }

    void AudiereAudio::update()
    {
        if(!disabled)
        {
            if(++cleanupSteps == STEPS_TO_CLEANUP)
            {
                // Cleanup all inactive channels.
                // Reverse iteration so we can delete while we loop over things.
                for(int i = activeChannels.size() - 1; i >= 0; i--)
                {
                    if(activeChannels[i]->shouldDispose)
                    {
                        delete activeChannels[i];
                        activeChannels.pop_back();
                    }
                }
                cleanupSteps = 0;
            }

            // Update the audio device.
            device->update();
        }
    }

    double AudiereAudio::getMasterPitch() const
    {
        return masterPitch;
    }

    void AudiereAudio::setMasterPitch(double pitch)
    {
        masterPitch = pitch;
        for(uint32_t i = 0; i < activeChannels.size(); i++)
        {
            if(!activeChannels[i]->shouldDispose)
            {
                activeChannels[i]->setPitch(pitch);
            }
        }
        for(uint32_t i = 0; i < soundStreamPool.size(); i++)
        {
            if (soundStreamPool[i] && soundStreamPool[i]->isPlaying())
            {
                soundStreamPool[i]->setPitchShift((float) pitch);
            }
        }
        return;
    }

    Sound* AudiereAudio::loadSound(const std::string& filename)
    {
        if(disabled)
        {
            return nullptr;
        }

        audiere::FilePtr file = new AudierePlumFile(new File(filename, FileRead));
        if(!file.get())
        {
            return nullptr;
        }

        audiere::SampleSourcePtr sample = audiere::OpenSampleSource(file, audiere::FF_AUTODETECT);
        if(!sample)
        {
            return nullptr;
        }

        audiere::SampleBufferPtr buffer = audiere::CreateSampleBuffer(sample);
        if(!buffer)
        {
            return nullptr;
        }

        return new AudiereSound(buffer);
    }

    // Attempt to allocate a new stream for a sound effect,
    // reusing an existing array slot, where possible
    // Returns the handle index for the added stream, or -1 on failure (no sound, bad stream).
    // Handle may become invalidated if the stream becomes null, or the sound stream finishes playing.
    int AudiereAudio::addSoundStream(audiere::OutputStreamPtr stream)
    {
        if(disabled)
        {
            return -1;
        }
        if(!stream)
        {
            return -1;
        }

        int found = -1;
        // Reuse streams where possible
        // Cleanup along the way.
        for (uint32_t i = 0; i < soundStreamPool.size(); ++i)
        {
            if (soundStreamPool[i] && !soundStreamPool[i]->isPlaying())
            {
                soundStreamPool[i] = NULL;
            }
            if(!soundStreamPool[i])
            {
                if(found == -1)
                {
                    found = i;
                }
            }
        }

        if(found != -1)
        {
            found;
            soundStreamPool[found] = stream;
            return found;
        }
        else
        {
            soundStreamPool.push_back(stream);
            return soundStreamPool.size() - 1;
        }
    }

    int AudiereAudio::playSound(Sound* sound, double volume)
    {
        if(disabled)
        {
            return 0;
        }
        if(!sound)
        {
            return 0;
        }

        auto audiereSound = (AudiereSound*) sound;
        auto stream = device->openStream(audiereSound->buffer->openStream());
        if(!stream)
        {
            return 0;
        }
        int handle = addSoundStream(stream);
        stream->play();

        return handle;
    }

    Channel* AudiereAudio::loadChannel(int handle)
    {
        if(handle >= 0 && (size_t) handle < soundStreamPool.size())
        {
            return loadChannel(soundStreamPool[handle]);
        }
        return nullptr;
    }

    Channel* AudiereAudio::loadChannel(audiere::OutputStreamPtr stream)
    {
        if(disabled || !stream)
        {
            return nullptr;
        }

        auto chan = new AudiereChannel(stream);
        activeChannels.push_back(chan);
        return chan;
    }

    Song* AudiereAudio::loadSong(const std::string& filename)
    {
        if(disabled)
        {
            return nullptr;
        }

        auto file = new AudierePlumFile(new File(filename, FileRead));
        if(!file)
        {
            return nullptr;
        }

        auto sample = audiere::OpenSampleSource(file, audiere::FF_AUTODETECT);
        if(!sample)
        {
            return nullptr;
        }

        auto song = new AudiereSong();
        song->sample = sample;
        song->volume = 1.0;
        song->channel = NULL;
        return song;
    }

    void AudiereAudio::playSong(Song* song)
    {
        AudiereSong* audiereSong = (AudiereSong*) song;
        if(disabled || !song)
        {
            return;
        }
        audiereSong->stop();

        auto stream = device->openStream(audiereSong->sample.get());
        if(!stream)
        {
            return;
        }

        audiereSong->channel = loadChannel(stream);
        stream->setRepeat(true);
        stream->setVolume((float) audiereSong->getVolume());
        stream->play();
    }
}