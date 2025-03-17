#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <vector>
#include <string>
#include <SDL_ttf.h>

class HighScore {
public:
    HighScore(const std::string& filename, TTF_Font* font, SDL_Renderer* renderer);
    ~HighScore();

    bool load();
    bool save();
    void addScore(int score);
    const std::vector<int>& getScores() const { return scores; }

    void render(int x, int y, TTF_Font* font) const; // Add the render method
private:
    std::string filename;
    std::vector<int> scores;
	TTF_Font* font;
	SDL_Renderer* renderer;
};

#endif