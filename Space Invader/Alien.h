#ifndef ALIEN_H
#define ALIEN_H

#include "GameObjects.h"
#include <vector>

class Alien : public GameObject {
public:
    Alien(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer, int numFrames); 
    ~Alien() override;

    void update(float deltaTime) override;
    void setDirection(int direction); 
    int getDirection() const { return direction; }
    void moveDown(int amount); //This is to move down
    bool shouldRemove = false;
    void setSpeed(float newSpeed) { speed = newSpeed; }
    float getSpeed() { return speed; }
	void render(SDL_Renderer* renderer) override;
    
private:
    float posX;
    int direction = 1; // 1 for right, -1 for left
    float speed;

    // Animation variables
    std::vector<SDL_Rect> animationFrames; // Source rectangles for each frame
    int currentFrame;                     // Index of the current frame
    float animationTimer;                 // Timer for animation
    float animationSpeed;               // How fast the animation plays (frames per second)
	int numFrames; // Number of frames in the animation
};

#endif