#include "GameObjects.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

GameObject::GameObject(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer) :
    rect({ x, y, w, h }), texture(nullptr)
{
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << texturePath << " - " << IMG_GetError() << std::endl;
        // Handle error
    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            // Handle error
        }
    }
}

GameObject::~GameObject() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void GameObject::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

bool GameObject::isColliding(const GameObject& other) const {
    return SDL_HasIntersection(&rect, &other.rect);
}