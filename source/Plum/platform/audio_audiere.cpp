#include <audiere.h>
#include <unordered_set>
#include <functional>

#include "../core/file.h"
#include "../core/audio.h"
#include "../core/engine.h"

namespace
{
    class FileWrapper : public audiere::RefImplementation<audiere::File>
    {
        public:
            FileWrapper(plum::File* f) 
            {
                file = f;
            }

            virtual ~FileWrapper()
            {
                delete file;
            }

            virtual int ADR_CALL read(void* buffer, int size)
            {
                return file->readRaw(buffer, size);
            }

            virtual bool ADR_CALL seek(int position, audiere::File::SeekMode mode)
            {
                plum::FileSeekMode m;
                switch (mode)
                {
                    case BEGIN: m = plum::SeekStart; break;
                    case CURRENT: m = plum::SeekCurrent; break;
                    case END: m = plum::SeekEnd; break;
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

    struct Playback
    {
        audiere::OutputStreamPtr stream;
        std::shared_ptr<bool> dispose;

        Playback()
            : stream(nullptr), dispose(nullptr)
        {
        }

        Playback(audiere::OutputStream* stream, const std::shared_ptr<bool>& dispose)
            : stream(stream), dispose(dispose)
        {
        }

        bool operator ==(const Playback& playback) const
        {
            return stream == playback.stream
                && dispose == playback.dispose;
        }
    };
}

namespace std
{
    template<> struct hash<Playback>
    {
        size_t operator()(const Playback& playback) const
        {
            return std::hash<audiere::OutputStream*>()(playback.stream.get())
                + std::hash<bool*>()(playback.dispose.get());
        }
    };
}

namespace plum
{
    class Channel::Impl
    {
        public:
            Playback pb;
    };

    Channel::Channel()
        : impl(new Impl())
    {
    }

    Channel::~Channel()
    {
        if(impl->pb.dispose)
        {
            *impl->pb.dispose = true;
        }
    }

    void Channel::play()
    {
        if(impl->pb.stream)
        {
            impl->pb.stream->play();
        }
    }

    void Channel::pause()
    {
        if(impl->pb.stream) 
        {
            impl->pb.stream->stop();
        }
    }

    void Channel::stop()
    {
        if(impl->pb.stream)
        {
            impl->pb.stream->stop();
            impl->pb.stream->reset();
        }
    }

    bool Channel::isPlaying() const
    {
        return impl->pb.stream ? impl->pb.stream->isPlaying() : false;
    }

    bool Channel::isLooped() const
    {
        return impl->pb.stream ? impl->pb.stream->getRepeat() : false;
    }

    double Channel::getPan() const
    {
        return impl->pb.stream ? impl->pb.stream->getPan() : 0.0;
    }

    double Channel::getPitch() const
    {
        return impl->pb.stream ? impl->pb.stream->getPitchShift() : 0.0;
    }

    double Channel::getVolume() const
    {
        return impl->pb.stream ? impl->pb.stream->getVolume() : 0.0;
    }

    void Channel::setPan(double value)
    {
        if(impl->pb.stream) impl->pb.stream->setPan((float) value);
    }

    void Channel::setPitch(double value)
    {
        if(impl->pb.stream) impl->pb.stream->setPitchShift((float) value);
    }

    void Channel::setVolume(double value)
    {
        if(impl->pb.stream) impl->pb.stream->setVolume((float) value);
    }

    void Channel::setLooped(bool value)
    {
        if(impl->pb.stream) impl->pb.stream->setRepeat(value);
    }



    class Sound::Impl
    {
        public:
            audiere::SampleSourcePtr sample;
            audiere::SampleBufferPtr buffer;
    };

    Sound::Sound()
        : impl(new Impl())
    {
    }

    Sound::~Sound()
    {
    }



    class Audio::Impl
    {
        public:
            Engine& engine;
            size_t update;

            bool disabled;
            double pan, pitch, volume;
            audiere::AudioDevicePtr device;
            std::unordered_set<Playback> playbacks;

            Impl(Engine& engine, bool disabled)
                : engine(engine), disabled(disabled), pan(0.0), pitch(1.0), volume(1.0)
            {
                if(!disabled)
                {
                    device = audiere::OpenDevice();
                }
                if(!device)
                {
                    device = audiere::OpenDevice("null", "");
                }

                update = engine.addUpdateHook([&]() {
                    if(disabled)
                    {
                        return;
                    }

                    auto& playbacks(playbacks);
                    for(auto it = playbacks.begin(), end = playbacks.end(); it != end;)
                    {
                        if((!it->stream->isPlaying() || it->stream->getRepeat()) && *it->dispose)
                        {
                            it = playbacks.erase(it);
                        }
                        else
                        {
                             ++it;
                        }
                    }
        
                    device->update();
                });
            }

            ~Impl()
            {
                engine.removeUpdateHook(update);
            }
    };

    Audio::Audio(Engine& engine, bool disabled)
        : impl(new Impl(engine, disabled))
    {
    }

    Audio::~Audio()
    {
    }

    void Audio::loadSound(const std::string& filename, bool streamed, Sound& sound)
    {
        if(impl->disabled)
        {
            return;
        }

        audiere::FilePtr file(new FileWrapper(new File(filename, FileRead)));
        if(file.get())
        {
            audiere::SampleSourcePtr sample(audiere::OpenSampleSource(file, audiere::FF_AUTODETECT));
            sound.impl->sample = sample;

            if(!streamed)
            {
                audiere::SampleBufferPtr buffer(audiere::CreateSampleBuffer(sample));
                sound.impl->buffer = buffer;
            }
        }
    }

    void Audio::loadChannel(const Sound& sound, Channel& channel)
    {
        if(impl->disabled)
        {
            return;
        }

        audiere::OutputStream* stream;
        if(sound.impl->buffer.get())
        {
            stream = impl->device->openStream(sound.impl->buffer->openStream());
        }
        else
        {
            stream = impl->device->openStream(sound.impl->sample.get());
        }

        std::shared_ptr<bool> dispose(new bool(false));
        Playback pb(stream, dispose);
        impl->playbacks.insert(pb);
        channel.impl->pb = pb;
    }

    double Audio::getPan() const
    {
        return impl->pan;
    }

    double Audio::getPitch() const
    {
        return impl->pitch;
    }

    double Audio::getVolume() const
    {
        return impl->volume;
    }

    void Audio::setPan(double value)
    {
        impl->pan = value;
    }

    void Audio::setPitch(double value)
    {
        impl->pitch = value;
    }

    void Audio::setVolume(double value)
    {
        impl->volume = value;
    }
}