#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include "Player.h"
#include "Background.h"
#include "Button.h" 
#include "Alien.h"
#include "AlienSwarm.h"
#include "Bullet.h"
#include "Score.h"
#include "LivesManager.h"
#include "HighScore.h"
#include "PowerUp.h"

class Button;
class Player;
class Background;
class Alien;
class Score;
class LivesManager;
class HighScore;
class PowerUp;
class AlienSwarm;

class Game {
public:
    enum class GameState {
        MENU,
        CHOOSE_MODE,
		SINGLE,
		DUO,
        PAUSE,
        GAME_OVER,
        HIGHSCORE
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
    bool singlePlayer = true;
    bool resetPlayerPosition = false;

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint32 lastFrameTime;
    float deltaTime;
    GameState currentState;
    Button* startButton = nullptr; 
    Button* quitButton = nullptr;
	HighScore* highScore = nullptr;
    Button* goToMenuButton = nullptr; 
    Button* pauseGoToMenuButton = nullptr;
	Button* scoreButton = nullptr;
	Button* replayButton = nullptr;
    Button* singleButton = nullptr;
	Button* duoButton = nullptr;
	Button* P1Win = nullptr;
	Button* P2Win = nullptr;
	SDL_Rect* border = nullptr;
    Player* player1 = nullptr;
	Player* player2 = nullptr;
    TTF_Font* gameFont = nullptr;
    TTF_Font* scoreFont = nullptr;
    Background* startBackground = nullptr;
    Background* gameBackground = nullptr;
    AlienSwarm* alienSwarm1 = nullptr;
    AlienSwarm* alienSwarm2 = nullptr;
    LivesManager* livesManager1 = nullptr;
    LivesManager* livesManager2 = nullptr;
    bool scoreAdded;
	bool sizeChanged = false;
    bool player1win = false;
    bool initialResetSwarm = false;
    std::vector<Bullet*> bullets; 
    std::vector<Bullet*> enemiesBullets;
	std::vector<PowerUp*> powerUps;
    Score* score = nullptr;
    void initializeAll();
};

#endif