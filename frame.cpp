#include "drawable.h"
#include "frame.h"
#include "ioMod.h"
#include "viewport.h"
#include "renderContext.h"

Frame::Frame( SDL_Texture* tex ) : 
  renderer(RenderContext::getInstance()->getRenderer()),
  texture( tex ),
  rect{0,0,0,0}
  //width(0), 
  //height(0)
{ SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h); }

Frame::Frame( const Frame& frame ) :
  renderer(frame.renderer),
  texture(frame.texture),
  rect(frame.rect)
  //width(frame.rect.w), 
  //height(frame.rect.h)
{ }


Frame& Frame::operator=(const Frame& rhs) {
  renderer = rhs.renderer;
  texture = rhs.texture;
  //width = rhs.rect.w;
  //height = rhs.rect.h;
  return *this;
}

void Frame::draw(int x, int y) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  SDL_Rect dest = {x, y, rect.w, rect.h };
  SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Frame::draw(int x, int y,float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int scaleHeight = scale * rect.h;
  int scaleWidth = scale * rect.w;
  SDL_Rect dest = {x, y, scaleWidth, scaleHeight};
  SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Frame::draw(int sx, int sy, int dx, int dy) const {
  SDL_Rect src = { sx, sy, rect.w, rect.h };    
  SDL_Rect dst = { dx, dy, rect.w, rect.h };
  SDL_RenderCopy(renderer, texture, &src, &dst);
}

Frame* Frame::crop(SDL_Rect sub)const{
  if(sub.x+sub.w > rect.w || sub.y+sub.h > rect.h){
   //if(sub.x+sub.w > width || sub.y+sub.h > height){
    std::cerr << "Attempted to crop image with invalid geometry."
              << std::endl
              << "(0,0 + "<<rect.w << "x"<<rect.h<<")"
              << " --> "
              << "("<<sub.x<<","<<sub.y<<" + "<<sub.w << "x"<<sub.h<<")"
              << std::endl;
    return nullptr; 
  }
  
  Frame* cloned = new Frame(*this);
  cloned->rect = sub;

  return cloned;
}
