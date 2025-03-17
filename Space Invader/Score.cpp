#include "Score.h"
#include <iostream>
#include "Utils.h"
#include <SDL_ttf.h>

Score::Score(TTF_Font* font, SDL_Renderer* renderer) :
    score(0), font(font), renderer(renderer), scoreTexture(nullptr), textColor({ 255, 255, 255, 255 }) // White
{
    updateTexture(); // Initialize the texture
}

Score::~Score() {
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
    }
}

void Score::increaseScore(int points) {
    score += points;
    updateTexture(); // Update the texture whenever the score changes
}

void Score::render(int x, int y) {
    if (scoreTexture) {
        int textW, textH;
        SDL_QueryTexture(scoreTexture, nullptr, nullptr, &textW, &textH); // Get text texture width and height
        SDL_Rect renderRect = { x, y, textW, textH };
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &renderRect); // Render the text
    }
}

void Score::reset() {
    score = 0;
    updateTexture(); // Update texture after resetting
}

void Score::updateTexture() {
    // Free previous texture
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr; // Set to nullptr after destroying
    }

    std::string scoreText = "Score " + std::to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (textSurface) {
        scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
    }
    else {
        std::cerr << "Unable to render score text! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
}