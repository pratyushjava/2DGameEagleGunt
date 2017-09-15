#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioMod.h"
#include <queue>

int fpsSum =0;
std::queue<int> fpsq;

Clock& Clock::getInstance() {
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  started(false), 
  paused(false), 
  FRAME_CAP_ON(Gamedata::getInstance().getXmlBool("frameCapOn")), 
  PERIOD(Gamedata::getInstance().getXmlInt("period")), 
  frames(0), 
  timeAtStart(0), 
  timeAtPause(0),
  currTicks(0), prevTicks(0), ticks(0),
  frame_max(Gamedata::getInstance().getXmlInt("maxFrames")),
  sumframes(0),
  fpsQue()
  {
  startClock();
  }

Clock::Clock(const Clock& c) :
  started(c.started), 
  paused(c.paused), 
  FRAME_CAP_ON(c.FRAME_CAP_ON), 
  PERIOD(c.PERIOD),
  frames(c.frames), 
  timeAtStart(c.timeAtStart), timeAtPause(c.timeAtPause),
  currTicks(c.currTicks), prevTicks(c.prevTicks), ticks(c.ticks),
  frame_max(c.frame_max),
  sumframes(c.sumframes),
  fpsQue(c.fpsQue)
  {
  startClock();
  }

void Clock::toggleSloMo() {
  throw( std::string("Slow motion is not implemented yet") );
}

unsigned int Clock::getTicks() const { 
  if (paused) return timeAtPause;
  else return SDL_GetTicks() - timeAtStart; 
}

unsigned int Clock::getElapsedTicks() { 
  if (paused) return 0;

  currTicks = getTicks();
  ticks = currTicks-prevTicks;

  if ( FRAME_CAP_ON ) {
    if ( ticks < PERIOD ) return 0;
    prevTicks = currTicks;
    return ticks;
  }
  else {
    prevTicks = currTicks;
    return ticks;
  }

}

int Clock::getFps() const { 
  if ( getSeconds() > 0 ) return frames/getSeconds();  
  else if ( getTicks() > 5000 and getFrames() == 0 ) {
    throw std::string("Can't getFps if you don't increment the frames");
  }
  else return 0;
}

void Clock::updateFpsSum()
{
  if ( !paused ) {

    int sec = getSeconds();
    int framerate = 0;
    if(sec > 0)
    {
		framerate = frames / sec;
	}
	else{
		framerate = 0;
	}
    if(fpsQue.size() == frame_max){
        int popval = fpsQue.back();
        sumframes -= popval;
        fpsQue.pop_back();
    }
    
    fpsQue.push_front(framerate);
    sumframes += framerate;
  }
}


float Clock::getAvgFps() const{ 
	
	return sumframes/frame_max;
	
}

void Clock::incrFrame() { 
  if ( !paused ) {
    ++frames; 
  }
}

void Clock::startClock() { 
  started = true; 
  paused = false; 
  frames = 0;
  timeAtPause = timeAtStart = SDL_GetTicks(); 
  prevTicks = 0;
}
void Clock::pause() {
  if( started && !paused ) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    paused = true;
  }
}
void Clock::unpause() {
  if( started && paused ) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    paused = false;
  }
}

