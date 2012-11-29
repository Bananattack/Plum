#pragma once

#include <string>

namespace plum
{
    class Sound 
    {
        public:
            virtual ~Sound() {}
    };

    // Channel data.
    // Do not allocate this yourself. Instead, use the audio class to do it for you.
    // When done, don't use delete. Use the dispose() function.
    class Channel
    {
        public:
            virtual ~Channel() {}

            virtual void dispose() = 0;
            virtual void resume() = 0;
            virtual void pause() = 0;
            virtual void stop() = 0;

            virtual bool isPlaying() const = 0;
            virtual double getVolume() const = 0;
            virtual double getPitch() const = 0;
            virtual void setVolume(double volume) = 0;
            virtual void setPitch(double pitch) = 0;
    };

    class Song
    {
        public:
            virtual ~Song() {}
            virtual void resume() = 0;
            virtual void pause() = 0;
            virtual void stop() = 0;

            virtual bool isPlaying() const = 0;
            virtual double getVolume() const = 0;
            virtual double getPitch() const = 0;
            virtual void setVolume(double volume) = 0;
            virtual void setPitch(double pitch) = 0;
    };

    class Audio
    {
        public:
            static Audio* create(bool disabled);

            virtual ~Audio() {}
            virtual double getMasterPitch() const = 0;
            virtual void setMasterPitch(double pitch) = 0;

            virtual Sound* loadSound(const std::string& filename) = 0;
            virtual Song* loadSong(const std::string& filename) = 0;
            virtual Channel* loadChannel(int handle) = 0;

            virtual int playSound(Sound* sound, double volume = 1.0) = 0;
            virtual void playSong(Song* song) = 0;

            virtual void update() = 0;
    };
}