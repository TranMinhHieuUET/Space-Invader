#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <functional> // For std::function (to handle button clicks)
#include <string>
#include "Game.h"

class Game;

class Button {
public:
    enum class ButtonType {
        START,
        QUIT,
        CONTINUE,
		GO_TO_MENU,
		SCORE
    };
    Button(int x, int y, int w, int h, const std::string& texturePath, Game* game, ButtonType type);
    ~Button();
    void render(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& event);
    ButtonType getType() const { return type; } // Get the button type

private:
    SDL_Rect rect; // Button position and size
    SDL_Texture* texture; // Button texture
    ButtonType type; // Button type 
    bool isHovered; // Track if the mouse is hovering over the button
    Game* game; // Declare game so that Button declaration can use
};

#endif