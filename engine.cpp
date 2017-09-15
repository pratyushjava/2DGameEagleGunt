#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "watersprite.h"
#include "shootingSprite.h"
#include "health.h"

class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

Engine::~Engine() { 
  std::cout << "Terminating program" << std::endl;
  
  if(player){
      delete player;
      player = NULL;
  }
  if(home) delete home;
  for(auto& sp : sprites) delete sp;
  for(auto& sp : scalableSprites) delete sp;
  for(auto& sp : enemies) delete sp;
  
}

Engine::Engine() :
  hud( Hud::getInstance() ),
  help(false),
  hudTime( Gamedata::getInstance().getXmlInt("hud/time") ),
  player(new ShootingSprite("eagle")),
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  maxCrow(Gamedata::getInstance().getXmlInt("crow/count")),
  renderer( rc->getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor")),
  mountain("mountain", Gamedata::getInstance().getXmlInt("mountain/factor")),
  city("city", Gamedata::getInstance().getXmlInt("city/factor")),
  sea("sea", Gamedata::getInstance().getXmlInt("sea/factor")),
  viewport( Viewport::getInstance() ),
  sprites(),
  enemies(),
  scalableSprites(),
  currentSprite(-1),
  makeVideo( false ),
  sound(),
  home(new Sprite("home")),
  playerWin(false),
  godMode(false)
{
  player->setPlayer();
  int maxShark = Gamedata::getInstance().getXmlInt("shark/count");
 
  int maxFish  = Gamedata::getInstance().getXmlInt("fish/count");
  int maxEnemies = Gamedata::getInstance().getXmlInt("enemy/count");
  for(int index =0 ; index < maxEnemies ; index++){
	enemies.push_back( new Player("enemy"));
	player->attach(enemies[index]);
  }
  
  for(int index =0 ; index < maxShark ; index++){
	sprites.push_back( new WaterSprite("shark"));
  }
  
  for(int index =0 ; index < maxFish ; index++){
	sprites.push_back( new WaterSprite("fish"));
  }

  float scale = Gamedata::getInstance().getXmlFloat("crow/scale/min");
  float maxScale = Gamedata::getInstance().getXmlFloat("crow/scale/max");
  float minScale = Gamedata::getInstance().getXmlFloat("crow/scale/min");
  for (int i = 0; i < maxCrow; ++i ) {
    float scaleDiff= maxScale - minScale;
    float incr= scaleDiff/maxCrow;
    scale = scale+incr;
    auto* s = new ScaledMultiSprite("crow",scale);
    scalableSprites.push_back(s);
  }
  
  std::vector<Drawable*>::iterator ptr = scalableSprites.begin();
  sort(ptr, scalableSprites.end(), SpriteLess());
  viewport.setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {

  sky.draw();
  int eachBgcrow = maxCrow /3;
  for (int i =0 ; i < eachBgcrow ; i++)
  {
	  scalableSprites[i]->draw();
  }
  
  mountain.draw();

  for (int i =eachBgcrow ; i < eachBgcrow*2 ; i++)
  {
	  scalableSprites[i]->draw();
  }
  
  city.draw();
  
  for (int i =eachBgcrow*2 ; i < maxCrow ; i++)
  {
	  scalableSprites[i]->draw();
  }
  
  sea.draw();
  clock.updateFpsSum();
  for(auto* s : sprites) s->draw();
  player->draw();
  for(auto* s : enemies) s->draw();
  
  if(clock.getSeconds() < hudTime || help){
    hud.displayHud();
  }
  
  if (playerWin){
	  hud.displayWin();
	  clock.pause();
  }
  
  if(Health::getInstance().getHealth() <= 0){
	  hud.displayLoose();
   }

  home->draw();
  Health::getInstance().draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  for(auto* s : scalableSprites) s->update(ticks);
  for(auto* s : enemies) s->update(ticks);
  
  sky.update();
  mountain.update();
  city.update();
  sea.update();
  
  for (unsigned i = 0; i < enemies.size(); ++i) {
    if(player->collidedWith(enemies[i]))
    {
		enemies[i]->setPosition(Vector2f(enemies[i]->getPosition()[0] + 500 , enemies[i]->getPosition()[1]));
		if(!godMode){
		player->explode();
		sound[8];
	    }
		static_cast<ShootingSprite*>(player)->collidedWith(enemies[i]);
	}
	if(static_cast<ShootingSprite*>(player)->collidedWith(enemies[i]))
    {
		static_cast<ShootingSprite*>(enemies[i])->explode();
		enemies[i]->setPosition(Vector2f(enemies[i]->getPosition()[0] + 500 , enemies[i]->getPosition()[1]));
	}
  }
  
  if(player->collidedWith(home))
  {
	playerWin = true;
  }
  
  if(Health::getInstance().getHealth() <= 0){
	  clock.pause();
  }
  player->update(ticks);
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  bool keyPress = false;
   while ( !done ) {
	   
	keystate = SDL_GetKeyboardState(NULL);
    while ( SDL_PollEvent(&event) ) {
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYUP) { 
        keyPress = false;
	  }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] && !keyPress) {
			keyPress = true;
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_Z] && !keyPress ) {
			keyPress = true;
          clock.toggleSloMo();
        }
        if(keystate[SDL_SCANCODE_F1] && !keyPress){
			keyPress = true;
			help = !help;
		}
        if ( keystate[SDL_SCANCODE_T] && !keyPress) {
			keyPress = true;
            switchSprite();
        }
        if ( keystate[SDL_SCANCODE_G] && !keyPress) {
			keyPress = true;
            godMode = !godMode;
            static_cast<Player *>(player)->setGodMode();
        }
        if ( keystate[SDL_SCANCODE_R] && !keyPress) {
			keyPress = true;
            clock.startClock();
            viewport.reset();
            Health::getInstance().reset();
            static_cast<ShootingSprite*>(player)->reset();
            for(auto* s : enemies) static_cast<Player*>(s)->reset();
            playerWin = false;
            godMode = false;
        }
        if ( keystate[SDL_SCANCODE_SPACE] && !keyPress) {
			keyPress = true;
            static_cast<ShootingSprite*>(player)->shoot();
            sound[6];
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo && !keyPress) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
          keyPress = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo && !keyPress) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
          keyPress = true;
        }
        else if(keystate[SDL_SCANCODE_R] && !keyPress){
	      IOmod::getInstance().setIsRed();
		  keyPress = true;
        }
      }
    }
    if((keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D]) || (keystate[SDL_SCANCODE_S] && keystate[SDL_SCANCODE_W])){
		player->stop();
	}
    else {
		if (keystate[SDL_SCANCODE_A] ) {
          		player->left();
		}        
		if (keystate[SDL_SCANCODE_D] ) {
          		player->right();
		} 
		if (keystate[SDL_SCANCODE_S] ) {
          		player->down();
		}
		if (keystate[SDL_SCANCODE_W] ) {
	      		player->up();
		}
	}
    
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
