#include "Bullet.h"

Bullet::Bullet(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, int speed) :
    GameObject(x, y, w, h, texturePath, renderer), speed(speed) {
}

Bullet::~Bullet() {}

void Bullet::update(float deltaTime) {
    rect.y += speed * deltaTime;

    // Check if bullet is off-screen
    if (rect.y + rect.h < 0 || rect.y > 980) {
        shouldRemove = true;
    }
}