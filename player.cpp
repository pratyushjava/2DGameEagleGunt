#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "viewport.h"
#include "collisionStrategy.h"
#include "health.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

Player::Player( const std::string& name) : 
  MultiSprite(name),
  strategy(),
  isExploding(false),
  initialVelocity(getVelocity()),
  explosion(NULL),
  godMode(false),
  isPlayer(false),
  isSmartSprite(false), //ai
  playerPos(0), //ai
  playerWidth(getFrame()->getWidth()/2), //ai
  playerHeight(getFrame()->getHeight()/2), //ai
  safeDistance(Gamedata::getInstance().getXmlInt("safeDistance")),  //ai 
  farDistance(Gamedata::getInstance().getXmlInt("farDistance")),
  currentMode(NORMAL),
  observers()
{}

Player::Player(const Player& s) :
  MultiSprite(s),
  strategy(s.strategy),
  isExploding(s.isExploding),
  initialVelocity(s.initialVelocity),
  explosion(s.explosion),
  godMode(s.godMode),
  isPlayer(s.isPlayer),
  isSmartSprite(s.isSmartSprite), //ai
  playerPos(s.playerPos), //ai
  playerWidth(s.playerWidth), //ai
  playerHeight(s.playerHeight), //ai
  safeDistance(s.safeDistance),  //ai 
  farDistance(s.farDistance),
  currentMode(s.currentMode),
  observers()
{}

Player::~Player(){
	if(explosion){
      delete explosion;
      explosion = NULL;
    }
}


void Player::stop(){
    setVelocityX(0.5 * getVelocityX());
    setVelocityY(0);
}

void Player::left(){
  if( getX() > 0){
	  setVelocityX(-initialVelocity[0]);
  }
}

void Player::right(){
  if( getX() < worldWidth-frameWidth){
	  setVelocityX(initialVelocity[0]);
  }
}

void Player::up(){  
  if( getY() > 0){
	  setVelocityY(initialVelocity[1]);
  }
}

void Player::down(){
  if( getY() < worldHeight-frameHeight){
	  setVelocityY(-initialVelocity[1]);
  }
}
void Player::draw() const{
	
	if(explosion && explosion->chunkCount() != 0){
		explosion->draw();
    }
    else{
		MultiSprite::draw();
	}
}

bool Player::collidedWith(const Drawable* d) {
  bool bRet = false;
  bRet = strategy.execute(*this, *d);
  if(bRet){
    if(!isExploding){
	  
      isExploding = true;
    }
  }
  else{
    isExploding = false;
  }
  return bRet;
}

void Player::explode(){
    if(true){
        if(!explosion){
            explosion = new ExplodingSprite(Sprite(getName(), getPosition(), getVelocity(), frames[currentFrame]));
		}
    }
}

void Player::update(Uint32 ticks) {
	if(explosion){
		explosion->update(ticks);
      if(explosion->chunkCount() == 0){
          delete explosion;
          if(!godMode && isPlayer){
              Health::getInstance().update();
		  }
          explosion = NULL;
      }
      return;
    }
    else{
		
	if(!isPlayer){
		smartSpriteUpdate();
		MultiSprite::update(ticks);
	}
	else{
		advanceFrame(ticks);
		Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.003;
		setPosition(getPosition() + incr);
		setPlayerPos(getPosition() + incr*2); //ai
		if( getX() < 0){
		  setVelocityX( abs( getVelocityX() ) );
		}
		if( getX() > worldWidth-frameWidth){
		  setVelocityX( -abs( getVelocityX() ) );
		}
		if( getY() < 0){
		  setVelocityY(abs( getVelocityY() ) );
		}
		if( getY() > worldHeight-frameHeight){
		  setVelocityY(-abs( getVelocityY() ) );
		}
    stop();
	}
  }
}



//ai smartsprite

void Player::goLeft()  {
  if (getX() > 0) setVelocityX( abs(getVelocityX()) ); 
}

void Player::goRight() { setVelocityX( -fabs(getVelocityX()) ); }
void Player::goUp()    { setVelocityY( fabs(getVelocityY()) ); }
void Player::goDown()  { setVelocityY( -fabs(getVelocityY()) ); }

void Player::smartSpriteUpdate() {
  float x= getX()+getFrame()->getWidth()/2;
  float y= getY()+getFrame()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );
  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
    }
  }
}

void Player::detach( Drawable* o ) {
  std::list<Drawable*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}



void Player::notify(){
  std::list<Drawable*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
	  static_cast<Player*>(*ptr)->setPlayerPos(playerPos);
      static_cast<Player*>(*ptr)->smartSpriteUpdate();
      ++ptr;
  }
}

void Player::reset(){
   setX(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
   setY(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y"));                    
   setVelocity( makeVelocity(Gamedata::getInstance().getXmlInt(getName()+"/speedX"),
                    Gamedata::getInstance().getXmlInt(getName()+"/speedY")));
                    
   isExploding = false;
   if(explosion) delete explosion;
   explosion = NULL;
   godMode = false;
}
