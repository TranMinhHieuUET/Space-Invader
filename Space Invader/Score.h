#ifndef Score_H
#define Score_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Score {
public:
    Score(TTF_Font* font, SDL_Renderer* renderer);
    ~Score();

    void increaseScore(int points);
    void render(int x, int y);
    void reset();
    int getScore() const { return score; } // Getter for the score

private:
    int score;
    SDL_Texture* scoreTexture;
    SDL_Color textColor;
    SDL_Renderer* renderer;
    TTF_Font* font;
    void updateTexture(); 
};

#endif