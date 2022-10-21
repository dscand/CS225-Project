#include "include/SDL_timer.h"

long double radians2degrees(long double radians) {
	return radians * (180.0/M_PI);
}
long double degrees2radians(long double degrees) {
	return degrees * (M_PI/180.0);
}

long double calcDistance(long double pos1X, long double pos1Y, long double pos2X, long double pos2Y) {
	return sqrt(((pos2X - pos1X) * (pos2X - pos1X)) + ((pos2Y - pos1Y) * (pos2Y - pos1Y)));
}
long double calcDirection(long double pos1X, long double pos1Y, long double pos2X, long double pos2Y) {
	return radians2degrees(atan2(pos1Y-pos2Y, pos1X-pos2X))+90;
}

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
LTimer::LTimer()
{
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}
void LTimer::start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}
void LTimer::stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}
void LTimer::pause()
{
	//If the timer is running and isn't already paused
	if( mStarted && !mPaused )
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}
void LTimer::unpause()
{
	//If the timer is running and paused
	if( mStarted && mPaused )
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}
int LTimer::getTicks()
{
	//The actual timer time
	int time = 0;

	//If the timer is running
	if( mStarted )
	{
		//If the timer is paused
		if( mPaused )
		{
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}
bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
	return mStarted;
}
bool LTimer::isPaused()
{
	//Timer is running and paused
	return mPaused && mStarted;
}