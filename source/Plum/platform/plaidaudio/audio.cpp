#include <iostream>
#include <unordered_set>
#include <functional>
#include <plaid/audio.h>
#include <plaid/audio/effects.h>

#include "../../core/file.h"
#include "../../core/audio.h"
#include "../../core/engine.h"

namespace
{
}

namespace plum
{
    class Channel::Impl
    {
        public:
            std::shared_ptr<plaidgadget::Audio> audio;
            plaidgadget::Sound master;
            plaidgadget::Ref<plaidgadget::Pan> panfx;
            plaidgadget::Ref<plaidgadget::Pitch> pitchfx;

            std::shared_ptr<bool> dispose;
            bool looped;
            double pan, pitch, volume;

            Impl()
                : audio(nullptr), master(nullptr), panfx(nullptr), pitchfx(nullptr), looped(false), pan(0.0), pitch(1.0), volume(1.0)
            {
            }

    };

    Channel::Channel()
        : impl(new Impl())
    {
    }

    Channel::~Channel()
    {
        if(impl->dispose)
        {
            *impl->dispose = true;
        }
    }

    void Channel::play()
    {
        if(impl->master)
        {
            impl->audio->play(impl->master);
        }
    }

    void Channel::pause()
    {
        if(impl->master)
        {
            impl->audio->pause(impl->master);
        }
    }

    void Channel::stop()
    {
        if(impl->master)
        {
            impl->audio->stop(impl->master);
        }
    }

    bool Channel::isPlaying() const
    {
        if(impl->master)
        {
            return impl->audio->playing(impl->master);
        }
        return false;
    }

    bool Channel::isLooped() const
    {
        return impl->looped;
    }

    double Channel::getPan() const
    {
        return impl->pan;
    }

    double Channel::getPitch() const
    {
        return impl->pitch;
    }

    double Channel::getVolume() const
    {
        return impl->volume;
    }

    void Channel::setPan(double value)
    {
        impl->pan = value;
    }

    void Channel::setPitch(double value)
    {
        impl->pitch = value;
    }

    void Channel::setVolume(double value)
    {
        impl->volume = value;
    }



    class Sound::Impl
    {
        public:
            plaidgadget::String filename;
            bool looped;
    };

    Sound::Sound()
        : impl(new Impl())
    {
    }

    Sound::~Sound()
    {
    }

    namespace
    {
        // VS2010 doesn't supply std::hash<std::shared_ptr<T>>.
        template<typename T> struct hash
        {
            size_t operator()(const std::shared_ptr<T>& p) const
            {
                return std::hash<T*>()(p.get());
            }
        };
    }

    class Audio::Impl
    {
        public:
            Impl(Engine& engine, bool disabled)
                : engine(engine), disabled(disabled), pan(0.0), pitch(1.0), volume(1.0),
                audio(new plaidgadget::Audio(disabled))
            {
                hook = engine.addUpdateHook([this](){ update(); });
            }

            ~Impl()
            {
            }

            void update()
            {
                if(disabled)
                {
                    return;
                }

                for(auto it = channels.begin(), end = channels.end(); it != end;)
                {
                    const auto& c(*it);

                    c->pitchfx->rate(float(c->pitch * pitch));
                    // TODO: panning
                    c->panfx->level(float(c->volume * volume));

                    if((!audio->playing(c->master) || c->looped) && *c->dispose)
                    {
                        it = channels.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }

                audio->update();
            }

            Engine& engine;
            std::shared_ptr<Engine::UpdateHook> hook;

            bool disabled;
            double pan, pitch, volume;

            std::shared_ptr<plaidgadget::Audio> audio;
            std::unordered_set<std::shared_ptr<Channel::Impl>, hash<Channel::Impl>> channels;
    };

    Audio::Audio(Engine& engine, bool disabled)
        : impl(new Impl(engine, disabled))
    {
    }

    Audio::~Audio()
    {
    }

    void Audio::loadSound(const std::string& filename, bool looped, Sound& sound)
    {
        if(impl->disabled)
        {
            return;
        }
        plaidgadget::String fn(filename.begin(), filename.end());
        sound.impl->filename = fn;
        sound.impl->looped = looped;
    }

    void Audio::loadChannel(const Sound& sound, Channel& channel)
    {
        if(impl->disabled)
        {
            return;
        }

        plaidgadget::Sound stream(impl->audio->stream(sound.impl->filename, sound.impl->looped));
        if(!stream.null())
        {
            plaidgadget::Ref<plaidgadget::Pitch> pitchfx(new plaidgadget::Pitch(stream));
            plaidgadget::Ref<plaidgadget::Pan> panfx(new plaidgadget::Pan(plaidgadget::Sound(pitchfx)));
            plaidgadget::Sound master = plaidgadget::Sound(panfx);

            channel.impl->looped = sound.impl->looped;
            channel.impl->audio = impl->audio;
            channel.impl->pitchfx = pitchfx;
            channel.impl->panfx = panfx;
            channel.impl->master = master;
            channel.impl->dispose.reset(new bool(false));
            impl->channels.insert(channel.impl);
        }
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