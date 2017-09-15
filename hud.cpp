#include "hud.h"
#include "ioMod.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "renderContext.h"
#include <sstream>
Hud& Hud::getInstance(){
    static Hud instance;
    return instance;
}

Hud::Hud():
    activeBullet(0),
    bulletPool(0),
    width(Gamedata::getInstance().getXmlInt("hud/width")),
    height(Gamedata::getInstance().getXmlInt("hud/height")),
    hudx(Gamedata::getInstance().getXmlInt("hud/start/x")),
    hudy(Gamedata::getInstance().getXmlInt("hud/start/y")),
    hudRect({hudx,hudy,width,height}),
    renderer( RenderContext::getInstance()->getRenderer() ),
    clock(Clock::getInstance()),
    io( IOmod::getInstance() ),
    textFontColor({0, 0, 0, 255})
{}

Hud::~Hud()
{}

int Hud::fontSize()const{
	float fWsize = width * 0.09;
	float fHsize = height * 0.09;
	if(fWsize < fHsize)
	{
		return (int)fWsize;
	}
	
	return (int)fHsize;
}


void Hud::displayHud() const{
    int fSize = fontSize();
    std::stringstream fps;
    fps << "Fps: " << clock.getFps();
    std::stringstream avgfps;
    avgfps << "Average fps: "<<clock.getAvgFps();
    std::stringstream time; 
    time<< "Elapsed seconds: "<<clock.getSeconds();
    std::stringstream actuveB;
    actuveB << "Active Bullet: " << activeBullet;
    std::stringstream bulletP;
    bulletP << "Bullet Pool: " << bulletPool;
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
    SDL_RenderFillRect( renderer, &hudRect );
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderDrawRect( renderer, &hudRect );
    io.writeText(fps.str(), hudx+15, hudy+15, textFontColor, fSize);
    io.writeText(avgfps.str(), hudx+15, hudy+40, textFontColor, fSize);
    io.writeText(time.str(), hudx+15, hudy+65, textFontColor, fSize);
    io.writeText(actuveB.str(), hudx+15, hudy+90, textFontColor, fSize);
    io.writeText(bulletP.str(), hudx+15, hudy+115, textFontColor, fSize);
    io.writeText("Press: ", hudx+15, hudy+140,textFontColor, fSize);
    io.writeText("a: Move Left ", hudx+15, hudy+165,textFontColor, fSize);
    io.writeText("d: Move Right", hudx+15, hudy+190,textFontColor, fSize);
    io.writeText("w: Move up", hudx+15, hudy+215,textFontColor, fSize);
    io.writeText("s: Move down: ", hudx+15, hudy+240, textFontColor, fSize);
    io.writeText("r: Reset Game: ", hudx+15, hudy+265, textFontColor, fSize);
    io.writeText("Space: Shooting: ", hudx+15, hudy+290, textFontColor, fSize);
    io.writeText("g: God Mode: ", hudx+15, hudy+315, textFontColor, fSize);
    
}



void Hud::displayLoose() const{
	SDL_Rect looseRect = {300,200,200,100};

    int fSize = fontSize();
    std::stringstream str1;
    str1 << "Game Over!! ";
    std::stringstream str2;
    str2 << "You Lose";
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
    SDL_RenderFillRect( renderer, &looseRect );
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderDrawRect( renderer, &looseRect );
    io.writeText(str1.str(), 320+15, 200+15, textFontColor, fSize);
    io.writeText(str2.str(), 320+15, 200+40, textFontColor, fSize);
}


void Hud::displayWin() const{
	SDL_Rect winRect = {300,200,300,100};

    int fSize = fontSize();
    std::stringstream str1;
    str1 << "Game Over!! ";
    std::stringstream str2;
    str2 << "You WIN";
    std::stringstream str3;
    str3 << "Destination Reached!! ";
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
    SDL_RenderFillRect( renderer, &winRect );
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderDrawRect( renderer, &winRect );
    io.writeText(str3.str(), 320+15, 200+15, textFontColor, fSize);
    io.writeText(str1.str(), 320+15, 200+45, textFontColor, fSize);
    io.writeText(str2.str(), 320+15, 200+75, textFontColor, fSize);	
}

