#include "Alien.h"
#include <iostream> 
#include "GameObjects.h"

Alien::Alien(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer) :
    GameObject(x, y, w, h, texturePath, renderer), direction(1), speed(100.0f), posX(static_cast<float>(x))
{
}

Alien::~Alien() {}

void Alien::update(float deltaTime) {

    // Update floating-point position
    posX += direction * speed * deltaTime;

    // Convert to integer for rendering
    rect.x = static_cast<int>(posX);

    
}

void Alien::moveDown(int amount)
{
    rect.y += amount;

    // Increase alien speed everytime it move down
    speed += 10.0f;
}

void Alien::setDirection(int newDirection) {
    direction = newDirection; 
}