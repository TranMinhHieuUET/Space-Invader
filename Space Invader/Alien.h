#ifndef ALIEN_H
#define ALIEN_H

#include "GameObjects.h"

class Alien : public GameObject {
public:
    Alien(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer);
    ~Alien() override;

    void update(float deltaTime) override;
    void setDirection(int direction); 
    int getDirection() const { return direction; }
    void moveDown(int amount); //This is to move down
    bool shouldRemove = false;


private:
    float posX;
    int direction = 1; // 1 for right, -1 for left
    int speed;

};

#endif