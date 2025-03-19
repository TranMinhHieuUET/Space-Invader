#ifndef POWERUP_H
#define POWERUP_H

#include <SDL.h>
#include <string>
#include "GameObjects.h" // Assuming PowerUp inherits from GameObject
#include <vector>
#include <stdlib.h>
#include "Alien.h"


class PowerUp : public GameObject {
public:
    enum class Type {
        EXTRA_LIFE,
        SPEED_BOOST,
        RAPID_FIRE,
		INVINCIBILITY
    };

    PowerUp(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, Type type);
    ~PowerUp() override;

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override; // Add a render method
	Type getType() const { return type; } // Get the power-up type
    bool shouldRemove = false; // Flag to indicate if the power-up should be removed
	static PowerUp* spawnPowerUp(int x, int y, SDL_Renderer* renderer); // Spawn the power-up

private:
    Type type;
    float fallSpeed = 300.0f;  // Speed at which the power-up falls
};
#endif