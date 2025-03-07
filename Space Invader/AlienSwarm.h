#ifndef ALIENSWARM_H
#define ALIENSWARM_H

#include <vector>
#include "Alien.h"
#include "GameObjects.h"

class AlienSwarm {
public:
    AlienSwarm(SDL_Renderer* renderer);
    ~AlienSwarm();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void reset(); // Reset swarm 
    std::vector<Alien*>& getAliens() { return aliens; }
private:
    std::vector<Alien*> aliens;
    SDL_Renderer* renderer; 
    int horizontalDirection;
    int moveDownDistance;
};
#endif