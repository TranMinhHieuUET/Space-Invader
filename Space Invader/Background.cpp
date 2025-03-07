#include "Background.h"
#include <SDL_image.h>
#include <iostream>
#include "Game.h"

Background::Background(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer)
    : rect({ x, y, w, h }), texture(nullptr) {
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << texturePath << " - " << IMG_GetError() << std::endl;
    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        }
    }
}

Background::~Background() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Background::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}