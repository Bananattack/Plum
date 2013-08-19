#ifndef PLUM_TIMER_H
#define PLUM_TIMER_H

#include <memory>

namespace plum
{
    enum class TimerSpeed
    {
        Normal,
        Fast,
        Slow
    };

    class Engine;
    class Timer
    {
        public:
            static const int FastForwardMultiplier = 4;
            static const int SlowMotionDivisor = 4;

            static const int DefaultMaxDelta = 5;

            Timer(Engine& engine);
            ~Timer();

            void reset();

            TimerSpeed getSpeed() const;
            unsigned int getMaxDelta() const;
            unsigned int getTime() const;
            unsigned int getDelta() const;
            unsigned int getFPS() const;

            void setSpeed(TimerSpeed speed);
            void setMaxDelta(unsigned int value);

            class Impl;
            std::shared_ptr<Impl> impl;
    };
}

#endif