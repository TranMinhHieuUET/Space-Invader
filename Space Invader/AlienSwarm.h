#ifndef ALIENSWARM_H
#define ALIENSWARM_H

#include <vector>
#include "Alien.h"
#include "GameObjects.h"
#include "Bullet.h"
#include <stdlib.h>
#include "Game.h"

class Game;

class AlienSwarm {
public:
    AlienSwarm(SDL_Renderer* renderer, std::vector<Bullet*>& enemiesBullets, Game* game, bool isP1);
    ~AlienSwarm();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void reset(); // Reset swarm 
    std::vector<Alien*>& getAliens() { return aliens; }
    void increaseSpeed(float increase); 
    void shoot();
    void increaseShootingSpeed(float increase);
    void increaseResetCounter();
    void resetResetCounter() { swarmResetCounter = 0; };
	void increaseNumOfShooter();
    void resetNumOfShooter() { numOfShooter = 1; };

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
	int swarmResetCounter;
    int numOfShooter;
    Game* game;
	bool isP1;
};
#endif