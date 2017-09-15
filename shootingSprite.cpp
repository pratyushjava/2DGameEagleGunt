#include "shootingSprite.h"
#include "gamedata.h"

ShootingSprite::ShootingSprite(const std::string& name) :
  Player(name),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets( bulletName ),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") )
{ }

ShootingSprite::ShootingSprite(const ShootingSprite& s) :
  Player(s),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed)
{ }

void ShootingSprite::shoot() { 
    Vector2f vel = getVelocity();
    float x;
    float y = getY()+ frameHeight/2;
    if(currentFrame < numberOfFrames/2) {        
      x = getX() + frameWidth - 40;
      vel[0] += minSpeed;
    }
    else {
      x=getX();
      vel[0] -= minSpeed;
    }
    vel[1] *=0;
  
    bullets.shoot(Vector2f(x,y), vel);
}

bool ShootingSprite::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}

void ShootingSprite::draw() const { 
  Player::draw();
  bullets.draw();
}

void ShootingSprite::update(Uint32 ticks) { 
  Player::update(ticks);
  bullets.update(ticks);
}

void ShootingSprite::reset(){
   Player::reset();
   bullets.reset();
} 

