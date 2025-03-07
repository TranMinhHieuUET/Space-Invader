#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <memory> // For std::unique_ptr
#include "Button.h" // Include the Button class header
#include <string>

class Button;

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
    bool running() { return isRunning; };
    void setGameState(GameState newState) { currentState = newState; };
    void setRunning(bool running) { isRunning = running; };
    SDL_Renderer* getRenderer() { return renderer; }

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint32 lastFrameTime;
    float deltaTime;
    GameState currentState;
    std::unique_ptr<Button> startButton; // Declare startButton as a unique_ptr
    std::unique_ptr<Button> quitButton;  // Declare quitButton as a unique_ptr
    std::unique_ptr<Button> goToMenuButton;  // Declare goToMenuButton as a unique_ptr
    void renderButton();
};

#endif