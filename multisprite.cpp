#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"


void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
if (timeSinceLastFrame > frameInterval) {
		if (getVelocityX() >= 0) {
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
Vector2f MultiSprite::makeVelocity(int vx, int vy)const{
	float a = Gamedata::getInstance().getRandInRange(vx-50, vx+50);
	float b = Gamedata::getInstance().getRandInRange(vy-50, vy+50);
	a = a * (rand()%2 ?-1:1);
	b = b * (rand()%2 ?-1:1);
	vx += a;
	vy += b;
	return Vector2f(a,b);
}

Vector2f MultiSprite::makeVelocity(int vx, int vy,float scale) const {


  float v_x = (scale +0.0002)*vx;
  float v_y = (scale +0.0002)*vy;
  return Vector2f(v_x,v_y);
}

MultiSprite::MultiSprite( const std::string& name) :

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
{
	setScale(1.0);	 
}

MultiSprite::MultiSprite( const std::string& name, float scale) :

  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 						
				
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"), scale)
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
{ 
setScale(scale);	
}

MultiSprite::MultiSprite(const MultiSprite& s) :
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
  { 
	  setScale(s.getScale());
  }

void MultiSprite::draw() const { 
  frames[currentFrame]->draw(getX(), getY());
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.0005;
  setPosition(getPosition() + incr);

  if ( getY() < 50) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > 0.50 *( worldHeight-frameHeight)) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 50) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > 0.85 * (worldWidth-frameWidth)) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
