#ifndef LIVESMANAGER_H
#define LIVESMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class LivesManager {
public:
    LivesManager(int initialLives, TTF_Font* font, SDL_Renderer* renderer);
    void loseLife();
    int getLives() const;
    bool isGameOver() const;
    void render(int x, int y);
    void reset();

private:
    int lives;
    SDL_Renderer* renderer;
    TTF_Font* font;
};
#endif