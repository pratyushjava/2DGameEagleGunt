#ifndef SCALEDMULTISPRITE__H
#define SCALEDMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "multisprite.h"

class ScaledMultiSprite : public MultiSprite {
public:
  ScaledMultiSprite(const std::string&,float);
  virtual void draw() const;
  inline float getScale()const{ return scale; }
  void  setScale(float s){ scale = s; }
  
private:
	float scale;
};
#endif
