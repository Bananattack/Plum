#ifndef PLUM_TIMER_H
#define PLUM_TIMER_H

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
		Uint32 maxFrameGap;
		Uint32 tickLast;
		Uint32 secondLast;
		Uint32 fractionalGap;
		Uint32 gap;
		Uint32 fps;
		Uint32 time;
		Uint32 updateCount;

		Timer();
		void reset();
		void processInput(Input& fastForward, Input& slowMotion);
		void update();
	};
}

#endif
