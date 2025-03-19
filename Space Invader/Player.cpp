#include "Player.h"
#include <iostream>
#include "Game.h"
#include "GameObjects.h"
#include "Bullet.h"

Player::Player(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, int speed, std::vector<Bullet*>& bullets) :
    GameObject(x, y, w, h, texturePath, renderer), speed(speed), shootCooldown(0.5f), lastShootTime(0.0f), initialX(x), initialY(y), 
	moveDirection(0), isSpacebarDown(false), posX(static_cast<float>(x)), bullets(bullets), renderer(renderer), invincibilityTime(0.0f), isInvincible(false),
    invincibilityDuration(2.0f), showSprite(true), flickerTimer(0.0f), flickerInterval(0.1f) {
}


Player::~Player() {}

void Player::update(float deltaTime) {
    // Update floating-point position
    posX += moveDirection * speed * deltaTime;

    // Convert to integer for rendering
    rect.x = static_cast<int>(posX);

    // Keep within bounds (update posX too)
    if (rect.x < 0) {
        rect.x = 0;
        posX = 0.0f;
    }
    if (rect.x + rect.w > 1710) {
        rect.x = 1710 - rect.w;
        posX = static_cast<float>(1710 - rect.w);
    }

    // Update invincibility timer
    if (isInvincible) {
        invincibilityTime += deltaTime;
        if (invincibilityTime >= invincibilityDuration) {
            isInvincible = false;
            invincibilityTime = 0.0f;
        }

        // Flickering Logic (ONLY while invincible)
        flickerTimer += deltaTime;
        if (flickerTimer >= flickerInterval) {
            showSprite = !showSprite; // Toggle visibility
            flickerTimer = 0.0f; // Reset timer
        }
    }
    else {
        showSprite = true;
    }

    // Handle shooting
    if (isSpacebarDown && lastShootTime >= shootCooldown) {
        bullets.push_back(new Bullet(rect.x + rect.w / 2 - 2, rect.y - 10, 16, 24, "Resource/bullet.png", renderer, -300)); // Negative speed for upward movement
        lastShootTime = 0.0f;
        // Play shoot sound here (using SDL_mixer)
    }
    lastShootTime += deltaTime;
}

void Player::render(SDL_Renderer* renderer) {
	if (showSprite) {
		GameObject::render(renderer);
	}
}

void Player::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
            if (event.type == SDL_KEYDOWN) {
                moveDirection = -1; // Change direction to left
            }
            else { // KEYUP
                if (moveDirection == -1) moveDirection = 0; // Change direction to 0 when left button is not pressed
            }
            break;
        case SDLK_RIGHT:
            if (event.type == SDL_KEYDOWN) {
                moveDirection = 1; // Change direction to right
            }
            else { // KEYUP
                if (moveDirection == 1) moveDirection = 0; // Change direction to 0 when right button is not pressed
            }
            break;
        case SDLK_SPACE:
            // Check if spacebar is pressed
            if (event.type == SDL_KEYDOWN) {
                isSpacebarDown = true;
            }
            else {
                isSpacebarDown = false;
            }
            break;
        }
    }
}
