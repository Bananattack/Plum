#include "plum.h"

namespace Plum
{
	Timer::Timer()
	{
		reset();
	}

	void Timer::reset()
	{
		speed = TimerSpeedNormal;
		maxFrameGap = TIMER_MAX_FRAME_GAP_DEFAULT;
		time = SDL_GetTicks();
		tickLast = time;
		secondLast = tickLast;
		fractionalGap = 0;
		gap = 0;
		fps = 0;
		updateCount = 0;
	}

	void Timer::processInput(Input& fastForward, Input& slowMotion)
	{
		if(fastForward.isPressed())
		{
			speed = TimerSpeedFastForward;
		}
		else if(slowMotion.isPressed())
		{
			speed = TimerSpeedSlowMotion;
		}
		else
		{
			speed = TimerSpeedNormal;
		}
	}

	void Timer::update()
	{
		time = SDL_GetTicks();
		updateCount++;
		if(time - secondLast > 999)
		{
			fps = updateCount;
			updateCount = 0;
			secondLast = time;
			printf("FPS: %u\n", fps);
		}
		if(time - tickLast > 0)
		{
			switch(speed)
			{
				case TimerSpeedFastForward: gap = time / (10 / TIMER_FAST_MULTIPLIER) - tickLast / (10 / TIMER_FAST_MULTIPLIER); break;
				case TimerSpeedNormal: gap = time / 10 - tickLast / 10; break;
				case TimerSpeedSlowMotion: gap = time / (10 * TIMER_SLOW_DIVISOR) - tickLast / (10 * TIMER_SLOW_DIVISOR); break;
			}
			if(speed != TimerSpeedFastForward)
			{
				gap = PLUM_MIN(maxFrameGap, gap);
			}
			tickLast = time;
		}
		else
		{
			gap = 0;
		}
	}
}
