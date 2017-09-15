#ifndef HEALTH__H
#define HEALTH__H

#include <SDL.h>
#include <SDL_image.h>
#include "renderContext.h"
#include "gamedata.h"
class Health {
public:
  static Health& getInstance();
  
  void draw() ;
  void update();
  void reset() { currentLength = 100; }
  int getHealth() { return currentLength; }
  
private:
  int x;
  int y;
  int width;
  int height;
  int currentLength;
  int increments;
  SDL_Renderer * renderer;
  void drawRect(int x, int y, int w, int h, float Percent);
  
  Health();
  Health(const Health&);
  Health& operator=(const Health&);
};

#endif
