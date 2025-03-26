#ifndef PLAYER_H
#define PLAYER_H

#include "GameObjects.h"
#include <vector>
#include "Bullet.h"
#include <string>
#include "Game.h"

class Game;

class Player : public GameObject {
public:
    Player(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, int speed, std::vector<Bullet*>& bullets, 
		SDL_Keycode leftKey, SDL_Keycode rightKey, SDL_Keycode shootKey, bool isP1, Game* game); // Add keycodes for movement and shooting for 2 player
    ~Player() override;

	int speed; // Public so that we can change it in Game.cpp
	float shootCooldown; // Public so that we can change it in Game.cpp
    void update(float deltaTime) override;
    void handleEvent(const SDL_Event& event);
	bool isInvincible; // Flag for invincibility
    float invincibilityTime;      // Timer for invincibility
	void render(SDL_Renderer* renderer) override;
	void setPosition(int x); // Set player position
    void shoot(); // Handle shooting bullet
    void resetSpeed(); // Reset player speed
    void resetMovement(); // Reset player movement

private:
    std::vector<Bullet*>& bullets; // Reference to the game's bullets
    float posX;
    float lastShootTime;
    int moveDirection; // -1 for left, 1 for right, 0 for no movement
    bool isShootKeyDown;
    float invincibilityDuration;  // How long invincibility lasts 
    bool showSprite;             // For flickering
    float flickerTimer;
    float flickerInterval;
    SDL_Renderer* renderer;
    SDL_Keycode leftKey;    // Key to move left
    SDL_Keycode rightKey;   // Key to move right
    SDL_Keycode shootKey;   // Key to shoot
	bool isP1; // Flag for player 1
	Game* game; // Declare game so that Player can use singlePlayer flag
    int initialSpeed; // Store initial speed
};

#endif