#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <functional> // For std::function (to handle button clicks)
#include <string>
#include "Game.h"
#include <SDL_mixer.h>

class Game;

class Button {
public:
    enum class ButtonType {
        START,
        QUIT,
        REPLAY,
		GO_TO_MENU,
        SINGLEPLAYER,
        DUOPLAYER,
		SCORE,
        GUIDE,
        ARROW
    };
    Button(int x, int y, int w, int h, const std::string& texturePath, Game* game, ButtonType type);
    // Constructor for image-based buttons (ARROW)
    Button(int x, int y, int w, int h, const std::string& normalTexturePath, const std::string& hoverTexturePath, Game* game, ButtonType type);
    ~Button();
    void render(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& event);
    ButtonType getType() const { return type; } // Get the button type

private:
    SDL_Rect rect; // Button position and size
    SDL_Texture* texture; // Button texture
    SDL_Texture* normalTexture; // Normal texture (not hovered) for the arrow button
    SDL_Texture* hoverTexture; // Hovered texture for the arrow button
    ButtonType type; // Button type 
    bool isHovered; // Track if the mouse is hovering over the button
    bool isArrow; // Flag for arrow button
    Game* game; // Declare game so that Button declaration can use
};

#endif