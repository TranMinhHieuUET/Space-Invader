#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

SDL_Texture* loadTexture(const std::string& filePath, SDL_Renderer* renderer);

#endif