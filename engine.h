#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include <algorithm>
#include "scaledMultisprite.h"
#include "hud.h"
#include "sound.h"
class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  Hud& hud;
  bool help;
  unsigned int hudTime;
  Player* player;
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  int maxCrow;
  SDL_Renderer * const renderer;
  World sky;
  World mountain;
  World city;
  World sea;
  Viewport& viewport;
  std::vector<Drawable*> sprites;
  std::vector<Drawable*> enemies;
  std::vector<Drawable*> scalableSprites;
  int currentSprite;
  bool makeVideo;
  SDLSound  sound;
  Sprite* home;
  bool playerWin;
  bool godMode;
  void draw() const;
  void update(Uint32);
  


  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};
