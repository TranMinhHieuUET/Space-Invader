#include "AlienSwarm.h"
#include <iostream>
#include "Alien.h"
#include "Bullet.h"
#include <stdlib.h>

static int rows = 5;
static int cols = 11;
static int alienWidth = 78;
static int alienHeight = 66;
static int hPadding = 10;
static int startX = 50;
static int startY = 50;

AlienSwarm::AlienSwarm(SDL_Renderer* renderer, std::vector<Bullet*>& enemiesBullets) : renderer(renderer), horizontalDirection(1), moveDownDistance(50), baseAlienSpeed(100.0f),
speedMultiplier(1.0f), timeSinceLastShot(0.0f), shootInterval(2.0f), enemiesBullets(enemiesBullets), swarmResetCounter(0), numOfShooter(1)
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
    timeSinceLastShot += deltaTime;

    // Shoot a bullet
    if (timeSinceLastShot > shootInterval)
    {
        shoot();
        timeSinceLastShot = 0;
    }

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

    //remove destroyed aliens 
    for (int i = 0; i < aliens.size(); ++i) {
        if (aliens[i]->shouldRemove) {
            delete aliens[i];
            aliens.erase(aliens.begin() + i);
            --i; // Adjust index after erasing
        }
    }
}


void AlienSwarm::render(SDL_Renderer* renderer) {
    // Render aliens
    for (Alien* alien : aliens) {
        alien->render(renderer);
    }

    // Render enemiesBullets
    for (Bullet* bullet : enemiesBullets) {
        bullet->render(renderer);
    }
}

void AlienSwarm::reset() {
    // Clear existing aliens
    for (Alien* alien : aliens) {
        delete alien;
    }
    aliens.clear();

    // Calculate current speed
    float currentSpeed = baseAlienSpeed * speedMultiplier; 

    // Create new aliens
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = startX + col * (alienWidth + hPadding);
            int y = startY + row * (alienHeight + hPadding);
            Alien* newAlien = new Alien(x, y, alienWidth, alienHeight, "Resource/alien_spritesheet.png", renderer, 2);
            newAlien->setSpeed(currentSpeed); 
            aliens.push_back(newAlien);
        }
    }
    horizontalDirection = 1;
}

void AlienSwarm::increaseSpeed(float increase) {
    speedMultiplier += increase;
}

void AlienSwarm::shoot() {
    // Check if there is any alien
    if (aliens.empty()) {
        return; 
    }

    srand(static_cast<unsigned int>(time(0))); // Seed random number generator

    // Alien shooting
	for (int i = 0; i < numOfShooter; i++) {
        // Create random index to choose an alien to shoot from
        int shooterIndex = rand() % (aliens.size());
        Alien* shooter = aliens[shooterIndex];

        // Create a bullet at the alien's position
        int bulletX = shooter->getRect().x + shooter->getRect().w / 2 - 10; // Center bullet
        int bulletY = shooter->getRect().y + shooter->getRect().h;
        enemiesBullets.push_back(new Bullet(bulletX, bulletY, 20, 20, "Resource/alien_bullet.png", renderer, 400)); // Adjust size and speed
	}
}

void AlienSwarm::increaseShootingSpeed(float increase) {
	shootInterval -= increase;
}

void AlienSwarm::increaseResetCounter() {
	swarmResetCounter++;
}

void AlienSwarm::increaseNumOfShooter() {
    // Calculate number of shooters
    if (swarmResetCounter == 2 && numOfShooter < 15) {
        numOfShooter++;
        swarmResetCounter = 0;
    }
}