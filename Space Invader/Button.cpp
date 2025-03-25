#include "Button.h"
#include <SDL_image.h>
#include <iostream>
#include "Game.h"

Button::Button(int x, int y, int w, int h, const std::string& texturePath, Game* game, ButtonType type) // Button constructor
    : rect({ x, y, w, h }), type(type), isHovered(false), texture(nullptr), game(game) { 
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << texturePath << " - " << IMG_GetError() << std::endl;
    }
    else {
        texture = SDL_CreateTextureFromSurface(game->getRenderer(), surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        }
    }
}

Button::~Button() { // Button destructor
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Button::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }

    // Hover effect
    if (isHovered) {
        // Save the current draw color
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

        // Set the draw color to yellow
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White border

        // Create and draw the border
        for (int i = 0; i < 5; i++) { // Adjust thickness 
            SDL_Rect borderRect = { rect.x - i, rect.y - i, rect.w + 2 * i, rect.h + 2 * i };
            SDL_RenderDrawRect(renderer, &borderRect);
        }

        // Reset the draw color to the original color
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }
}

void Button::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        // Get mouse position
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Check if mouse is over the button
        bool wasHovered = isHovered;
        isHovered = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);

        // Handle click
        if (isHovered && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            switch (type) {
            case ButtonType::START:
                game->setGameState(Game::GameState::CHOOSE_MODE); // Move from pause to choosing single or duo
                break;
            case ButtonType::QUIT:
                game->setRunning(false); // Quit the game
                break;
            case ButtonType::GO_TO_MENU:
                game->setGameState(Game::GameState::MENU); // Move from pause to menu
                // Play menu music
                Mix_HaltMusic(); // Stop any currently playing music
                if (Mix_PlayMusic(game->menuMusic, -1) == -1) {
                    std::cerr << "Mix_PlayMusic error: " << Mix_GetError() << std::endl;
                }
                break;
			case ButtonType::SCORE:
				game->setGameState(Game::GameState::HIGHSCORE); // Move from menu to score screen
                break;
            case ButtonType::REPLAY:
                if (game->singlePlayer == true) {
                    game->setGameState(Game::GameState::SINGLE); // Move from game over to single player game
				}
				else {
					game->setGameState(Game::GameState::DUO); // Move from game over to duo player game
				}
				break;
            case ButtonType::SINGLEPLAYER:
				game->singlePlayer = true; // Set single player mode
				game->setGameState(Game::GameState::SINGLE); // Move from choosing mode to single player game
                // Play game music
                Mix_HaltMusic(); // Stop any currently playing music
                if (Mix_PlayMusic(game->backgroundMusic, -1) == -1) {
                    std::cerr << "Mix_PlayMusic error: " << Mix_GetError() << std::endl;
                }
				break;
			case ButtonType::DUOPLAYER:
				game->singlePlayer = false; // Set duo player mode
				game->setGameState(Game::GameState::DUO); // Move from choosing mode to duo player game
                // Play game music
                Mix_HaltMusic(); // Stop any currently playing music
                if (Mix_PlayMusic(game->backgroundMusic, -1) == -1) {
                    std::cerr << "Mix_PlayMusic error: " << Mix_GetError() << std::endl;
                }
				break;
            }
        }
    }
}