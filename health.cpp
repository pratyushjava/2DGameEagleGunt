#include "health.h"

Health& Health::getInstance(){
    static Health instance;
    return instance;
}

Health::Health() :
  x(Gamedata::getInstance().getXmlFloat("health/x")),
  y(Gamedata::getInstance().getXmlFloat("health/y")),
  width(Gamedata::getInstance().getXmlFloat("health/width")),
  height(Gamedata::getInstance().getXmlFloat("health/height")),
  currentLength(Gamedata::getInstance().getXmlFloat("health/length")), 
  increments(Gamedata::getInstance().getXmlFloat("health/increments")), renderer(RenderContext::getInstance()->getRenderer())
{
}

void Health::drawRect(int x, int y, int w, int h, float Percent) { 
   SDL_Color FGColor({0, 255, 0, 200});
   SDL_Color BGColor({255, 0, 0, 255});
   SDL_Rect bgrect = { x, y, w, h }; 
   SDL_SetRenderDrawColor(renderer, BGColor.r, BGColor.g, BGColor.b, BGColor.a); 
   SDL_RenderFillRect(renderer, &bgrect); 
   SDL_SetRenderDrawColor(renderer, FGColor.r, FGColor.g, FGColor.b, FGColor.a); 
   
   int pw =  (int)((float)w * (Percent/100)); 
   SDL_Rect fgrect = { x, y, pw, h }; 
   SDL_RenderFillRect(renderer, &fgrect); 
} 

void Health::draw()  {
  drawRect(x, y , width , height , (float) currentLength );
}

void Health::update() {
    
  if ( currentLength > 0) {
    currentLength -= increments;
  }
  draw();
}

