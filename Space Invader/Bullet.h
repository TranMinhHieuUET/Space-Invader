#ifndef BULLET_H
#define BULLET_H

#include "GameObjects.h"
#include <string>

class Bullet : public GameObject {
public:
    Bullet(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, int speed);
    ~Bullet() override;

    void update(float deltaTime) override;

    bool shouldRemove = false; // Flag for removal

private:
    int speed;
};

#endif
