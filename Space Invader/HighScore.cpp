#include "HighScore.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>

HighScore::HighScore(const std::string& filename, TTF_Font* font, SDL_Renderer* renderer)
    : filename(filename), font(font), renderer(renderer) 
{
    scores.resize(10, 0);
}

HighScore::~HighScore() {

}

bool HighScore::load() {
    std::ifstream file(filename);
    if (!file.is_open()) { // Error handling
        std::cerr << "HighScore: Could not open file for loading: " << filename << std::endl;
        return false; 
    }

    scores.clear(); // Clear any existing scores
    int score;
    int count = 0;
    while (file >> score && count < 10) { // Read up to 10 scores
        scores.push_back(score);
        count++;
    }

    file.close();

    // If read fewer than 10 scores, fill the rest with 0
    while (scores.size() < 10) {
        scores.push_back(0);
    }
    // Sort after loading (in case the file was not sorted)
    std::sort(scores.begin(), scores.end(), std::greater<int>());


    return true; // Return true to indicate success
}

bool HighScore::save() {
    std::ofstream file(filename);
    if (!file.is_open()) { // Error handling
        std::cerr << "HighScore: Could not open file for saving: " << filename << std::endl;
        return false;
    }

    for (int score : scores) { // Save score into txt file
        file << score << "\n";
    }

    file.close();
    return true;
}

void HighScore::addScore(int score) {
    scores.push_back(score);
    std::sort(scores.begin(), scores.end(), std::greater<int>());
    scores.resize(10);
}

void HighScore::render(int x, int y, TTF_Font* font) const {
    if (!font || !renderer) {
        std::cerr << "HighScore::render(): A font or renderer is null!" << std::endl;
        return;
    }

    const int lineHeight = TTF_FontHeight(font) + 10; // Use FontHeight for spacing
    SDL_Color White = { 255, 255, 255, 255 };

    //  Render "HIGH SCORE" Title (Centered) 
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "HIGH SCORE", White);
    if (!titleSurface) {
        std::cerr << "HighScore::render(): TTF_RenderText_Solid (title) failed: " << TTF_GetError() << std::endl;
    }
    else {
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        SDL_FreeSurface(titleSurface);

        if (!titleTexture) {
            std::cerr << "HighScore::render(): SDL_CreateTextureFromSurface (title) failed: " << SDL_GetError() << std::endl;
        }
        else {
            SDL_Rect titleRect;
            SDL_QueryTexture(titleTexture, nullptr, nullptr, &titleRect.w, &titleRect.h);
            titleRect.x = x - titleRect.w / 2; // Center the title
            titleRect.y = y;
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
            SDL_DestroyTexture(titleTexture);

            y += titleRect.h + 20; // Move down below the title
        }
    }

    // Render Scores
    for (size_t i = 0; i < scores.size(); ++i) {
        std::stringstream ss;
        ss << i + 1 << "   " << scores[i];

        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, ss.str().c_str(), White); 
        if (!surfaceMessage) {
            std::cerr << "HighScore::render(): TTF_RenderText_Solid (score) failed: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_FreeSurface(surfaceMessage);

        if (!messageTexture) {
            std::cerr << "HighScore::render(): SDL_CreateTextureFromSurface (score) failed: " << SDL_GetError() << std::endl;
            continue;
        }

        SDL_Rect messageRect;
        SDL_QueryTexture(messageTexture, nullptr, nullptr, &messageRect.w, &messageRect.h);
        messageRect.x = x - messageRect.w / 2; // Center the score text
        messageRect.y = y + static_cast<int>(i) * lineHeight;


        SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
        SDL_DestroyTexture(messageTexture);
    }
}