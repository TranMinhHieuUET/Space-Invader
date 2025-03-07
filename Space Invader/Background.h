#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include <string>
#include "Game.h"

class Game;

class Background {
public:
    Background(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer);
    ~Background();
    void render(SDL_Renderer* renderer);

private:
    SDL_Rect rect; // Background position and size
    SDL_Texture* texture; // Background texture
};

#endif