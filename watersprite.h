
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class WaterSprite : public Drawable {
public:
  WaterSprite(const std::string&);
  WaterSprite(const WaterSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  
protected:
  const std::vector<Frame *> frames;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  void advanceFrame(Uint32 ticks);
  Vector2f makeVelocity(int,int) const;
};

