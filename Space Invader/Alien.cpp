#include "Alien.h"
#include <iostream> 
#include "GameObjects.h"

Alien::Alien(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, int numFrames) :
    GameObject(x, y, w, h, texturePath, renderer), direction(1), speed(100.0f), posX(static_cast<float>(x)),
    currentFrame(0), animationTimer(0.0f), animationSpeed(2.0f), numFrames(numFrames) 
{
    animationFrames.resize(numFrames); // Make sure animationFrames has the right size
    for (int i = 0; i < numFrames; ++i) {
        animationFrames[i].x = i * 416,5;
        animationFrames[i].y = 0;
        animationFrames[i].w = 416,5;
        animationFrames[i].h = 353;
    }
}

Alien::~Alien() {}

void Alien::update(float deltaTime) {

    // Update floating-point position
    posX += direction * speed * deltaTime;

    // Convert to integer for rendering
    rect.x = static_cast<int>(posX);

    // Update animation
    animationTimer += deltaTime;
    if (animationTimer >= 1.0f / animationSpeed) { // Check if it's time to change frames
        currentFrame = (currentFrame + 1) % numFrames;  // Cycle through frames
        animationTimer -= 1.0f / animationSpeed;      // Reset timer 
    }
}

void Alien::render(SDL_Renderer* renderer) {
    if (texture) { // Make sure texture exist
        SDL_RenderCopy(renderer, texture, &animationFrames[currentFrame], &rect); // Use current frame's source rect
    }
}

void Alien::moveDown(int amount)
{
    rect.y += amount;

    // Increase alien speed everytime it move down
    speed += 15.0f;
}

void Alien::setDirection(int newDirection) {
    direction = newDirection; 
}