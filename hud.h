#ifndef HUD__H
#define HUD__H
#include <string>
#include <SDL.h>
#include <stdint.h>
#include "clock.h"
#include "ioMod.h"
class Hud {
public:
  static Hud& getInstance();  
  ~Hud();
  void displayHud() const;
  void displayLoose() const;
  void displayWin() const;
  void setActiveBullet(int activeB){activeBullet = activeB;}
  void setBulletpool(int bullet){bulletPool = bullet;}
private:
  int activeBullet;
  int bulletPool;
  const int width;
  const int height;
  int hudx;
  int hudy;
  SDL_Rect hudRect;
  SDL_Renderer* renderer;
  Hud();
  Hud(const Hud&);
  Hud& operator=(const Hud&);
  Clock& clock;
  const IOmod& io;
  SDL_Color textFontColor;
  int fontSize()const;
};
#endif
