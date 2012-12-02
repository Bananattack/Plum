#ifndef PLUM_AUDIO_H
#define PLUM_AUDIO_H

#include <string>
#include <memory>

namespace plum
{
    class Engine;
    class Channel;
    class Sound;
    class Audio;

    class Channel
    {
        public:
            Channel();
            ~Channel();

            void play();
            void pause();
            void stop();

            bool isPlaying() const;
            bool isLooped() const;
            double getPan() const;
            double getPitch() const;
            double getVolume() const;
            void setPan(double value);
            void setPitch(double value);
            void setVolume(double value);
            void setLooped(bool value);

        private:
            friend class Sound;
            friend class Audio;
            class Impl;
            std::shared_ptr<Impl> impl;
    };

    class Sound 
    {
        public:
            Sound();
            ~Sound();

        private:
            friend class Audio;
            class Impl;
            std::shared_ptr<Impl> impl;
    };

    class Audio
    {
        public:
            Audio(Engine& engine, bool disabled);
            ~Audio();

            void loadSound(const std::string& filename, bool streamed, Sound& sound);
            void loadChannel(const Sound& sound, Channel& channel);

            double getPan() const;
            double getPitch() const;
            double getVolume() const;
            void setPan(double value);
            void setPitch(double value);
            void setVolume(double value);

        private:
            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif