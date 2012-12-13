#include <algorithm>

#include <SDL.h>

#include "../../core/timer.h"
#include "../../core/engine.h"

namespace plum
{
    class Timer::Impl
    {
        public:
            Impl(Engine& engine)
                : engine(engine)
            {
                speed = TimerSpeedNormal;
                maxDelta = DefaultMaxDelta;

                unsigned int t = SDL_GetTicks();
                currentTick = t;
                previousTick = t;
                previousSecond = t;
                delta = 0;
                fps = 0;
                frames = 0;
                elapsed = 0;

                hook = engine.addUpdateHook([this](){ update(); });
            }

            ~Impl()
            {
            }

            void update()
            {
                ++frames;
                currentTick = SDL_GetTicks();
                if(currentTick - previousSecond > 999)
                {
                    fps = frames;
                    frames = 0;
                    previousSecond = currentTick;
                }
                if(currentTick - previousTick > 0)
                {
                    switch(speed)
                    {
                        case TimerSpeedFastForward: delta = currentTick / (10 / FastForwardMultiplier) - previousTick / (10 / FastForwardMultiplier); break;
                        case TimerSpeedNormal: delta = currentTick / 10 - previousTick / 10; break;
                        case TimerSpeedSlowMotion: delta = currentTick / (10 * SlowMotionDivisor) - previousTick / (10 * SlowMotionDivisor); break;
                    }
                    if(speed != TimerSpeedFastForward)
                    {
                        delta = std::min(maxDelta, delta);
                    }
                    previousTick = currentTick;
                    elapsed += delta;
                }
                else
                {
                    delta = 0;
                }
            }

            Engine& engine;
            std::shared_ptr<Engine::UpdateHook> hook;

            TimerSpeed speed;
            unsigned int maxDelta;

            unsigned int currentTick;
            unsigned int previousTick;
            unsigned int previousSecond;
            unsigned int delta;
            unsigned int fps;

            unsigned int frames;
            unsigned int elapsed;
    };

    Timer::Timer(Engine& engine)
        : impl(new Impl(engine))
    {
    }

    Timer::~Timer()
    {
    }

    TimerSpeed Timer::getSpeed() const
    {
        return impl->speed;
    }

    unsigned int Timer::getMaxDelta() const
    {
        return impl->maxDelta;
    }

    unsigned int Timer::getTime() const
    {
        return impl->elapsed;
    }

    unsigned int Timer::getDelta() const
    {
        return impl->delta;
    }

    unsigned int Timer::getFPS() const
    {
        return impl->fps;
    }

    void Timer::setSpeed(TimerSpeed speed)
    {
        impl->speed = speed;
    }

    void Timer::setMaxDelta(unsigned int value)
    {
        impl->delta = value;
    }
}
