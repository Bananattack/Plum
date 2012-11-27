#pragma once

namespace plum
{

    struct Sound
    {
        audiere::SampleBufferPtr buffer;
        Sound() : buffer(0)
        {
        }

        ~Sound()
        {
        }
    };

    // Channel data.
    // Do not allocate this yourself. Instead, use the audio class to do it for you.
    // When done, don't use delete. Use the dispose() function.
    struct Channel
    {
        audiere::OutputStreamPtr stream;
        bool shouldDispose;

        Channel(audiere::OutputStreamPtr channel)
        {
            this->stream = channel;
            shouldDispose = false;
        }
        ~Channel() {}

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
        
        bool isPlaying()
        {
            return stream->isPlaying();
        }

        void stop()
        {
            stream->stop();
            stream->reset();
        }

        double getVolume()
        {
            return stream->getVolume();
        }

        void setVolume(double volume)
        {
            stream->setVolume((float) volume);
        }

        double getPitch()
        {
            return stream->getPitchShift();
        }

        void setPitch(double pitch)
        {
            stream->setPitchShift((float) pitch);
        }
    };


    struct Song
    {
        audiere::SampleSourcePtr sample;
        Channel* channel;
        double volume;

        Song() : sample(0), channel(0), volume(1.0)
        {
        }

        ~Song()
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
        
        bool isPlaying()
        {
            return channel && channel->isPlaying();
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

        double getVolume()
        {
            return this->volume;
        }

        void setVolume(double volume)
        {
            this->volume = volume;
            if(channel)
            {
                channel->setVolume(volume);
            }
        }

        double getPitch()
        {
            return channel->getPitch();
        }

        void setPitch(double pitch)
        {
            channel->setPitch(pitch);
        }
    };

    struct Audio
    {
        class Exception : public std::exception
        {
            public:
                Exception(const std::string& message)
                    : msg(message)
                {
                }

                virtual const char* what() const throw ()
                {
                    return msg.c_str();
                }

                virtual ~Exception() throw ()
                {
                }

            private:
                std::string msg;
        };

    private:
        static const int STEPS_TO_CLEANUP = 50;
        int cleanupSteps;

        double masterPitch;

        bool disableSound;
        audiere::AudioDevicePtr audioDevice;
        std::vector<audiere::OutputStreamPtr> soundStreamPool;
        std::vector<Channel*> activeChannels;

        int addSoundStream(audiere::OutputStreamPtr stream);
        Channel* createChannel(audiere::OutputStreamPtr stream);
    public:
        Audio()
            : audioDevice(0)
        {
        }

        void startup(bool disableSound = false);
        void shutdown();
        void update();
        void setMasterPitch(double pitch);

        Sound* loadSound(std::string filename);
        Sound* loadSound(const char* filename);
        int playSound(Sound* sound, double volume = 1.0);

        Channel* createChannel(int handle);

        Song* loadSong(std::string filename);
        Song* loadSong(const char* filename);
        void playSong(Song* song);
    };
}