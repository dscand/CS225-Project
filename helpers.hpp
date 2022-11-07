#ifndef helpers
#define helpers

#include <math.h>

#include "include/SDL_timer.h"

long double radians2degrees(long double radians);
long double degrees2radians(long double degrees);

long double calcDistance(long double pos1X, long double pos1Y, long double pos2X, long double pos2Y);
long double calcDirection(long double pos1X, long double pos1Y, long double pos2X, long double pos2Y);

// LTimer class from lazyfoo.net
class LTimer {
	public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		int getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

	private:
		//The clock time when the timer started
		int mStartTicks;

		//The ticks stored when the timer was paused
		int mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};

#endif