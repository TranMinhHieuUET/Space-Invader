#ifndef ALIENSWARM_H
#define ALIENSWARM_H

#include <vector>
#include "Alien.h"
#include "GameObjects.h"
#include "Bullet.h"
#include <stdlib.h>

class AlienSwarm {
public:
    AlienSwarm(SDL_Renderer* renderer, std::vector<Bullet*>& enemiesBullets);
    ~AlienSwarm();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void reset(); // Reset swarm 
    std::vector<Alien*>& getAliens() { return aliens; }
    void increaseSpeed(float increase); 
    void shoot();

private:
    std::vector<Alien*> aliens;
    std::vector<Bullet*>& enemiesBullets;
    SDL_Renderer* renderer; 
    int horizontalDirection;
    int moveDownDistance;
    float speedMultiplier;
    float baseAlienSpeed;     
    float timeSinceLastShot;
    float shootInterval;
};
#endif