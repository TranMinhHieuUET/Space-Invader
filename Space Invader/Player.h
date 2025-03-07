#ifndef PLAYER_H
#define PLAYER_H

#include "GameObjects.h"
#include <vector>
//#include "Bullet.h"
#include <string>

class Player : public GameObject {
public:
    Player(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, int speed);
    ~Player() override;

    void update(float deltaTime) override;
    void handleEvent(const SDL_Event& event, SDL_Renderer* renderer);
    void resetPosition(); // Add this
private:
    float posX;
    int speed;
    float shootCooldown;
    float lastShootTime;
    int initialX;  // Store initial X position
    int initialY; //  Store initial Y position
    int moveDirection; // -1 for left, 1 for right, 0 for no movement
};

#endif