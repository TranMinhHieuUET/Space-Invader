#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL.h>
#include <string>

class GameObject {
public:
    GameObject(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer);
    virtual ~GameObject(); // Virtual Destructor (It ensures that the correct destructor is called, even if you're using a pointer to the base class)

    virtual void update(float deltaTime) = 0; // Pure virtual function, this means that any class that inherits from GameObject 
                                              //must provide its own implementation of update. This forces derived classes to define how they behave each frame.
    virtual void render(SDL_Renderer* renderer); // Render object to screen. Virtual so that derived class can override it.

    SDL_Rect getRect() const { return rect; } // Get object bounding rectangle to use for collision, size, position, ....
    bool isColliding(const GameObject& other) const; // Check if object is colliding with other object.

protected:
    SDL_Rect rect; // To store object position (x,y) and size (width, height).
    SDL_Texture* texture; // The object texture
};

#endif