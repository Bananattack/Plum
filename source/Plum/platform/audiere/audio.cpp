#include <iostream>
#include <audiere.h>
#include <unordered_set>
#include <functional>

#include "../../core/file.h"
#include "../../core/audio.h"
#include "../../core/engine.h"

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
}

namespace plum
{
    class Channel::Impl
    {
        public:
            audiere::OutputStreamPtr stream;
            std::shared_ptr<bool> dispose;
            double pan, pitch, volume;

            Impl()
                : stream(nullptr), dispose(nullptr), pan(0.0), pitch(1.0), volume(1.0)
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
        if(impl->stream)
        {
            impl->stream->play();
        }
    }

    void Channel::pause()
    {
        if(impl->stream) 
        {
            impl->stream->stop();
        }
    }

    void Channel::stop()
    {
        if(impl->stream)
        {
            impl->stream->stop();
            impl->stream->reset();
        }
    }

    bool Channel::isPlaying() const
    {
        return impl->stream ? impl->stream->isPlaying() : false;
    }

    bool Channel::isLooped() const
    {
        return impl->stream ? impl->stream->getRepeat() : false;
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

    void Channel::setLooped(bool value)
    {
        if(impl->stream) impl->stream->setRepeat(value);
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
                : engine(engine), disabled(disabled), pan(0.0), pitch(1.0), volume(1.0)
            {
                if(!disabled)
                {
                    device = audiere::OpenDevice();
                }
                if(!device)
                {
                    disabled = true;
                    device = audiere::OpenDevice("null", "");
                }

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

                    c->stream->setPitchShift(float(c->pitch * pitch));
                    c->stream->setVolume(float(c->volume * volume));

                    if((!c->stream->isPlaying() || c->stream->getRepeat()) && *c->dispose)
                    {
                        it = channels.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
        
                device->update();
            }

            Engine& engine;
            std::shared_ptr<Engine::UpdateHook> hook;

            bool disabled;
            double pan, pitch, volume;
            audiere::AudioDevicePtr device;
            std::unordered_set<std::shared_ptr<Channel::Impl>, hash<Channel::Impl>> channels;
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

        channel.impl->stream = stream;
        channel.impl->dispose.reset(new bool(false));
        impl->channels.insert(channel.impl);
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