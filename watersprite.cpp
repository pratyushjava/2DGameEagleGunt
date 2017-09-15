#include "watersprite.h"
#include "gamedata.h"
#include "renderContext.h"


void WaterSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
if (timeSinceLastFrame > frameInterval) {
		if (getVelocityX() > 0) {
			currentFrame = (currentFrame + 1) % (numberOfFrames/2);
		}
		else {
			if (currentFrame > numberOfFrames / 2) {
				currentFrame = (currentFrame + 1) % numberOfFrames;
				if(currentFrame < numberOfFrames / 2){
					currentFrame = (numberOfFrames /2)  +1;
				}
			}
			else{
				currentFrame = (currentFrame + (numberOfFrames / 2) + 1) % numberOfFrames;
				if(currentFrame < numberOfFrames / 2){
					currentFrame = (numberOfFrames /2)  +1;
				}
			}
		}
		timeSinceLastFrame = 0;
	}
}

Vector2f WaterSprite::makeVelocity(int vx, int vy)const{
	float a = Gamedata::getInstance().getRandInRange(vx-50, vx+50);
	float b = Gamedata::getInstance().getRandInRange(vy-50, vy+50);
	a = a * (rand()%2 ?-1:1);
	b = b * (rand()%2 ?-1:1);
	vx += a * rand();
	vy += b * rand();
	return Vector2f(a,b);
}


WaterSprite::WaterSprite( const std::string& name) :

  Drawable(name, 
          Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 						
				
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( RenderContext::getInstance()->getFrames(name) ),


  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

WaterSprite::WaterSprite(const WaterSprite& s) :
  Drawable(s), 
  frames(s.frames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

void WaterSprite::draw() const { 
  frames[currentFrame]->draw(getX(), getY());
}

void WaterSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 280) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > 0.90 *( worldHeight-frameHeight)) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 50) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > 0.90 * (worldWidth-frameWidth)) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
