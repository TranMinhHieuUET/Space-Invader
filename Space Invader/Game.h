#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Button;
class Player;
class Background;
class Alien;

#include "Player.h"
#include "Background.h"
#include "Button.h" 
#include "Alien.h"
#include "AlienSwarm.h"
#include "Bullet.h"

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
    Button* startButton = nullptr; 
    Button* quitButton = nullptr;
    Button* goToMenuButton = nullptr; 
    Player* player = nullptr;
    Background* startBackground = nullptr;
    Background* gameBackground = nullptr;
    AlienSwarm* alienSwarm = nullptr;
    std::vector<Bullet*> bullets; 
    void initializeAll();
};

#endif