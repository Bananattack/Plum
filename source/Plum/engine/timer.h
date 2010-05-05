#pragma once

namespace Plum
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
		u32 maxFrameGap;
		u32 tickLast;
		u32 secondLast;
		u32 fractionalGap;
		u32 gap;
		u32 fps;
		u32 time;
		u32 updateCount;
        u32 ticks;

		Timer();
		void reset();
		void processInput(Input& fastForward, Input& slowMotion);
		void update();
	};
}
