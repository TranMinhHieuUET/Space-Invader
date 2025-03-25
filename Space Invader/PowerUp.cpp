#include "PowerUp.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "Alien.h"

PowerUp::PowerUp(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, Type type)
	: GameObject(x, y, w, h, texturePath, renderer), type(type)
{
}

PowerUp::~PowerUp() {}

void PowerUp::update(float deltaTime) {
    // Make the power-up fall down the screen
    rect.y += static_cast<int>(fallSpeed * deltaTime);
    // Remove the power-up if it goes off-screen
	if (rect.y > 1080) { // If y position is greater than the screen height
        shouldRemove = true;
    }
}

void PowerUp::render(SDL_Renderer* renderer) {
    GameObject::render(renderer); // Call the base class render function
}

PowerUp* PowerUp::spawnPowerUp(int x, int y, SDL_Renderer* renderer) {
    // Random chance (20%)
    if (rand() % 100 < 20) {
        // Choose a random power-up type
        int randIndex = rand() % 4;
        Type type = static_cast<Type>(randIndex);
        switch (type) {
		case Type::EXTRA_LIFE:
			return new PowerUp(x, y, 32, 32, "Resource/Power_Up/extra_life.png", renderer, type);
            break;
		case Type::SPEED_BOOST:
			return new PowerUp(x, y, 32, 32, "Resource/Power_Up/speed_boost.png", renderer, type);
			break;
		case Type::RAPID_FIRE:
			return new PowerUp(x, y, 32, 32, "Resource/Power_Up/rapid_fire.png", renderer, type);
			break;
		case Type::INVINCIBILITY:
			return new PowerUp(x, y, 32, 32, "Resource/Power_Up/invincibility.png", renderer, type);
            break;
        }
    }
    return nullptr; // Return nullptr if no power-up is spawned
}