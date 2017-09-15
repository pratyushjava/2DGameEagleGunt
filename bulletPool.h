#include <list>
#include "bullet.h"
#include "collisionStrategy.h"
#include "hud.h"
class BulletPool {
public:
  BulletPool(const std::string&);
  BulletPool(const BulletPool&);
  ~BulletPool();
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& vel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  bool shooting() const { return bulletList.empty(); }
  bool collidedWith(const Drawable* obj) const;
  void reset();
private:
  std::string name;
  CollisionStrategy* strategy;
  float frameInterval;
  float timeSinceLastFrame;
  mutable std::list<Bullet> bulletList; // BulletPool is a list of Bullet
  mutable std::list<Bullet> freeList;   // when a Bullet is out of range

  BulletPool& operator=(const BulletPool&);   // (Item 6)
};
