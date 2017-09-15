#include <iostream>
#include <cmath>
#include <functional>
#include "scaledMultisprite.h"


ScaledMultiSprite::ScaledMultiSprite( const std::string& name, float s) : MultiSprite(name, s), scale(s)
{ }

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void ScaledMultiSprite::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  frames[currentFrame]->draw(getX(), getY(), scale); 
}
