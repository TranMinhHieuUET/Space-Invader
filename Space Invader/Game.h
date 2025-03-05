#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>

class Game {
public:
    enum class GameState {
        MENU,
        PLAYING,
        PAUSE,
        GAME_OVER
    };
    Game();
    ~Game();
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() { return isRunning; }

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint32 lastFrameTime;
    float deltaTime;
    GameState currentState;
    void handleMenuEvents(const SDL_Event& event);
    void renderGameMenu();
};

#endif