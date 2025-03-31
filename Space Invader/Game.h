#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
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
        HIGHSCORE,
        GUIDE
    };
    Game();
    ~Game();
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() { return isRunning; };
    void setGameState(GameState newState);
    void setRunning(bool running) { isRunning = running; };
    SDL_Renderer* getRenderer() { return renderer; }
    bool singlePlayer = true;
    bool resetPlayerPosition = false;
    Mix_Chunk* shootSound = nullptr;
    Mix_Chunk* buttonPressedSound = nullptr;

private:
    // Declare all variables
    bool isRunning; // Flag to check if game is running
    float deltaTime; // Variable for smooth game update
    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint32 lastFrameTime;
    GameState currentState;
    HighScore* highScore = nullptr;
    // All ingame button
    Button* startButton = nullptr; 
    Button* quitButton = nullptr;
    Button* gameOverQuitButton = nullptr;
    Button* goToMenuButton = nullptr; 
    Button* pauseGoToMenuButton = nullptr;
	Button* scoreButton = nullptr;
	Button* replayButton = nullptr;
    Button* singleButton = nullptr;
    Button* duoButton = nullptr;
    Button* arrow = nullptr;
    Button* guideButton = nullptr;
    // Ingame background
    Background* P1Win = nullptr;
    Background* P2Win = nullptr;
    Background* gameTitle = nullptr;
    Background* gameOverTitle = nullptr;
    Background* guide = nullptr;
    Background* startBackground = nullptr;
    Background* gameBackground = nullptr;
    Background* gameOverBackground = nullptr;
	SDL_Rect* border = nullptr; // Border for duo mode
    Player* player1 = nullptr;
	Player* player2 = nullptr;
    TTF_Font* gameFont = nullptr;
    TTF_Font* scoreFont = nullptr;
    AlienSwarm* alienSwarm1 = nullptr;
    AlienSwarm* alienSwarm2 = nullptr;
    LivesManager* livesManager1 = nullptr;
    LivesManager* livesManager2 = nullptr;
    Mix_Chunk* playerHitSound = nullptr;
    Mix_Chunk* alienHitSound = nullptr;
    Mix_Chunk* powerUpSound = nullptr;
    Mix_Chunk* gameOverSound = nullptr;
    Mix_Music* backgroundMusic = nullptr;
    Mix_Music* menuMusic = nullptr;
    Mix_Music* currentMusic = nullptr;
    bool scoreAdded;
	bool sizeChanged = false;
    bool player1win;
    bool initialResetSwarm = false;
    bool speedResetted = false;
    bool gameOver = false;
    std::vector<Bullet*> bullets; 
    std::vector<Bullet*> enemiesBullets;
	std::vector<PowerUp*> powerUps;
    Score* score = nullptr;
    void initializeAll();
};

#endif