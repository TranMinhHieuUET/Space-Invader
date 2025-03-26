#include "Player.h"
#include <iostream>
#include "GameObjects.h"
#include "Bullet.h"

Player::Player(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, int speed, std::vector<Bullet*>& bullets,
    SDL_Keycode leftKey, SDL_Keycode rightKey, SDL_Keycode shootKey, bool isP1, Game* game) :
    GameObject(x, y, w, h, texturePath, renderer), speed(speed), shootCooldown(0.5f), lastShootTime(0.0f),
    moveDirection(0), isShootKeyDown(false), posX(static_cast<float>(x)), bullets(bullets), renderer(renderer), invincibilityTime(0.0f), isInvincible(false), initialSpeed(speed),
    invincibilityDuration(2.0f), showSprite(true), flickerTimer(0.0f), flickerInterval(0.1f), leftKey(leftKey), rightKey(rightKey), shootKey(shootKey), isP1(isP1), game(game) {
}


Player::~Player() {}

void Player::update(float deltaTime) {
    // Update floating-point position
    posX += moveDirection * speed * deltaTime;

    // Convert to integer for rendering
    rect.x = static_cast<int>(posX);

    // Keep within bounds (update posX too) (for single and duo player)
    if (game->singlePlayer == true) {
        if (rect.x < 0) {
            rect.x = 0;
            posX = 0.0f;
        }
        if (rect.x + rect.w > 1710) {
            rect.x = 1710 - rect.w;
            posX = static_cast<float>(1710 - rect.w);
        }
    }
    else {
		if (isP1 == true) {
			if (rect.x < 0) {
				rect.x = 0;
				posX = 0.0f;
			}
			if (rect.x + rect.w > 855) {
				rect.x = 855 - rect.w;
				posX = static_cast<float>(855 - rect.w);
			}
		}
		else {
			if (rect.x < 855) {
				rect.x = 855;
				posX = 855.0f;
			}
			if (rect.x + rect.w > 1710) {
				rect.x = 1710 - rect.w;
				posX = static_cast<float>(1710 - rect.w);
			}
		}
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
    lastShootTime += deltaTime; // Update last shoot time
}

void Player::shoot() {
    // Handle shooting
    if (isShootKeyDown && lastShootTime >= shootCooldown) {
        bullets.push_back(new Bullet(rect.x + rect.w / 2 - 2, rect.y - 10, 8, 12, "Resource/Player/bullet.png", renderer, -350)); // Negative speed for upward movement
        lastShootTime = 0.0f;
		Mix_PlayChannel(-1, game->shootSound, 0); // Play shoot sound
    }
}

void Player::render(SDL_Renderer* renderer) {
	if (showSprite) {
		GameObject::render(renderer);
	}
}

void Player::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) { // Do it this way for smooth movement
        if (event.key.keysym.sym == leftKey) {
            if (event.type == SDL_KEYDOWN) {
                moveDirection = -1; // Change direction to left
            }
            else { // KEYUP
                if (moveDirection == -1) moveDirection = 0; // Change direction to 0 when left button is not pressed
            }
        }
        else if (event.key.keysym.sym == rightKey) {
            if (event.type == SDL_KEYDOWN) {
                moveDirection = 1; // Change direction to right
            }
            else { // KEYUP
                if (moveDirection == 1) moveDirection = 0; // Change direction to 0 when right button is not pressed
            }
        }
        else if (event.key.keysym.sym == shootKey) {
            // Check if shootkey is pressed
            if (event.type == SDL_KEYDOWN) {
                isShootKeyDown = true;
            }
            else {
                isShootKeyDown = false;
            }
        }
    }
}

void Player::setPosition(int x) {
	rect.x = x;
	posX = static_cast<float>(x);
}

void Player::resetSpeed() {
    speed = initialSpeed;
}
