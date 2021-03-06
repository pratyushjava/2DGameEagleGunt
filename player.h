#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "multisprite.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"
class Player : public MultiSprite {
public:

  Player(const std::string&);
  Player(const Player&);
  virtual ~Player();
  virtual void draw() const;
  virtual void update(Uint32 ticks);  
  void explode();
  bool collidedWith(const Drawable* d);
  void stop();
  void left();
  void right();
  void up();
  void down();
  void setPlayer(){isPlayer = !isPlayer;};
  //ai smart sprites
  void smartSpriteUpdate();
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  float getSafeDistance() { return safeDistance;  }
  void setPlayerPos(const Vector2f& p) { playerPos = p; notify();}
  void setSmartSprite(bool isSmart) {isSmartSprite = isSmart;}
  //ai subject
  void attach( Drawable* o ){ observers.push_back(o); } 
  void detach( Drawable* o );
  void notify();
  void reset();
  void setGodMode(){godMode = !godMode;};
  bool isGodMOde(){return godMode;}
private:
  enum MODE {NORMAL, EVADE};
  PerPixelCollisionStrategy strategy;
  bool isExploding;
  Vector2f initialVelocity;
  Player& operator=(const Player&);
  ExplodingSprite* explosion;
  bool godMode;
  bool isPlayer;
  //ai smartsprite
  bool isSmartSprite;
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  float safeDistance;
  float farDistance;
  MODE currentMode;
  //ai subject
  std::list<Drawable*> observers;
};

#endif
