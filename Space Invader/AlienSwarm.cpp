#include "AlienSwarm.h"
#include <iostream>

AlienSwarm::AlienSwarm(SDL_Renderer* renderer) : renderer(renderer), horizontalDirection(1), moveDownDistance(50)
{
    reset(); // Initialize the aliens
}

AlienSwarm::~AlienSwarm() {
    for (Alien* alien : aliens) {
        delete alien;
    }
    aliens.clear();
}

void AlienSwarm::update(float deltaTime) {

    bool edgeReached = false;

    //move aliens and check bounds
    for (Alien* alien : aliens) {
        alien->setDirection(horizontalDirection);
        alien->update(deltaTime);
        if ((alien->getRect().x + alien->getRect().w > 1710 && horizontalDirection == 1) || (alien->getRect().x < 0 && horizontalDirection == -1)) { // Check if reach edge
            edgeReached = true; // Set the edgeReached = true;
        }
    }

    //if edge reached, move down and reverse direction
    if (edgeReached) {
        horizontalDirection *= -1;
        for (Alien* alien : aliens) {
            alien->moveDown(moveDownDistance); // Move all aliens down
        }
    }

    //remove destroyed aliens (will work when there is bullet)
    for (int i = 0; i < aliens.size(); ++i) {
        if (aliens[i]->shouldRemove) {
            delete aliens[i];
            aliens.erase(aliens.begin() + i);
            --i; // Adjust index after erasing
        }
    }
}


void AlienSwarm::render(SDL_Renderer* renderer) {
    for (Alien* alien : aliens) {
        alien->render(renderer);
    }
}

void AlienSwarm::reset() {
    // Clear existing aliens
    for (Alien* alien : aliens) {
        delete alien;
    }
    aliens.clear();

    // Create new aliens
    int rows = 5;
    int cols = 11;
    int alienWidth = 78;
    int alienHeight = 66;
    int hPadding = 10;
    int startX = 50;
    int startY = 50;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = startX + col * (alienWidth + hPadding);
            int y = startY + row * (alienHeight + hPadding);
            aliens.push_back(new Alien(x, y, alienWidth, alienHeight, "Resource/alien.png", renderer));
        }
    }
    horizontalDirection = 1;
}