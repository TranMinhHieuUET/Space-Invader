#include "AlienSwarm.h"
#include <iostream>
#include "Alien.h"
#include "Bullet.h"
#include <stdlib.h>
#include "Game.h"

static int alienWidth = 39;
static int alienHeight = 33;
static int hPadding = 5;

AlienSwarm::AlienSwarm(SDL_Renderer* renderer, std::vector<Bullet*>& enemiesBullets, Game* game, bool isP1) : renderer(renderer), horizontalDirection(1), moveDownDistance(50), 
baseAlienSpeed(80.0f), speedMultiplier(1.0f), timeSinceLastShot(0.0f), shootInterval(2.0f), enemiesBullets(enemiesBullets), swarmResetCounter(0), numOfShooter(1), game(game), isP1(isP1)
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
        // Keep within bounds (for single and duo player)
        if (game->singlePlayer == true) {
            if ((alien->getRect().x + alien->getRect().w > 1710 && horizontalDirection == 1) || (alien->getRect().x < 0 && horizontalDirection == -1)) { // Check if reach edge
                edgeReached = true; // Set the edgeReached = true;
            }
        }
        else {
            if (isP1 == true) {
                if ((alien->getRect().x + alien->getRect().w > 855 && horizontalDirection == 1) || (alien->getRect().x < 0 && horizontalDirection == -1)) { // Check if reach edge
                    edgeReached = true; // Set the edgeReached = true;
                }
            }
            else {
				if ((alien->getRect().x + alien->getRect().w > 1710 && horizontalDirection == 1) || (alien->getRect().x < 855 && horizontalDirection == -1)) { // Check if reach edge
					edgeReached = true; // Set the edgeReached = true;
				}
            }
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
    int startX;
    int rows, cols;
    if (game->singlePlayer == true) {
        startX = 50;
        rows = 5;
        cols = 10;
    }
    else {
        rows = 4;
        cols = 9;
		if (isP1 == true) {
			startX = 0;
		}
		else {
			startX = 855;
		}
    }
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = startX + col * (alienWidth + hPadding);
            int y = 50 + row * (alienHeight + hPadding);
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