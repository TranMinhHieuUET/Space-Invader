#include "LivesManager.h"
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <SDL.h>

LivesManager::LivesManager(int initialLives, TTF_Font* font, SDL_Renderer* renderer) : lives(initialLives), renderer(renderer), font(font) {}

void LivesManager::loseLife() {
    if (lives > 0) {
        lives--;
    }
}

int LivesManager::getLives() const {
    return lives;
}

bool LivesManager::isGameOver() const {
    return lives <= 0;
}
void LivesManager::render(int x, int y) {
    if (!renderer) {
        std::cerr << "Renderer is null in LivesManager::render" << std::endl;
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 }; // White
    std::string livesText = "Lives " + std::to_string(lives);
    SDL_Surface* surface = TTF_RenderText_Solid(font, livesText.c_str(), textColor);
    if (!surface)
    {
        std::cerr << "TTF Error:" << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        std::cerr << "Texture creation Error:" << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect textRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
void LivesManager::reset()
{
    lives = 3; 
}