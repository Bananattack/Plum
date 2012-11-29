#pragma once

#include "input.h"

namespace plum
{
    const int TIMER_FAST_MULTIPLIER = 4;
    const int TIMER_SLOW_DIVISOR = 4;
    const int TIMER_MAX_FRAME_GAP_DEFAULT = 5;
    enum TimerSpeed
    {
        TimerSpeedNormal,
        TimerSpeedFastForward,
        TimerSpeedSlowMotion
    };

    struct Timer
    {
        TimerSpeed speed;
        uint32_t maxFrameGap;
        uint32_t tickLast;
        uint32_t secondLast;
        uint32_t fractionalGap;
        uint32_t gap;
        uint32_t fps;
        uint32_t time;
        uint32_t updateCount;
        uint32_t ticks;

        Timer();
        void reset();
        void processInput(Input& fastForward, Input& slowMotion);
        void update();
    };
}
