#include "Game.h"
#include <iostream>
#include "Utils.h"
#include "Button.h"
#include "Player.h"
#include "Background.h"
#include "Alien.h"

// Button dimensions
static int buttonWidth = 300;
static int buttonHeight = 150;

// Get screen dimensions
static int windowWidth = 1710;
static int windowHeight = 980;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), lastFrameTime(0), deltaTime(0.0f), currentState(GameState::MENU), scoreAdded(false) {}

Game::~Game() {
    delete startButton;
    delete quitButton;
    delete goToMenuButton;
    delete scoreButton;
	delete replayButton;
    delete startBackground;
    delete gameBackground;
	delete pauseGoToMenuButton;
    delete player;
    delete alienSwarm;
    if (!bullets.empty()) {
        for (Bullet* bullet : bullets) {
            delete bullet;
        }
    }
    bullets.clear();
    for (Bullet* bullet : enemiesBullets) {
        delete bullet;
    }
    enemiesBullets.clear();
    delete livesManager;
	delete highScore;
    delete score;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Initialize SDL_Image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    //Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
    } 

    // Create window
    Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 

    // Create the all entities
    initializeAll();

    isRunning = true;
    lastFrameTime = SDL_GetTicks();
    deltaTime = 0.0f;

    return true;
}

void Game::initializeAll() {
    // Calculate button positions
    int centeredX = (windowWidth - buttonWidth) / 2;
	int centeredY = (windowHeight - buttonHeight) / 2;
    int menuButtonsY = 750;
    int startButtonX = centeredX - buttonWidth - 100;
    int scoreButtonX = centeredX;
    int quitButtonX = centeredX + buttonWidth + 100;
    int goToMenuButtonX = 100;
	

    // Initialize the background
    startBackground = new Background(0, 0, windowWidth, windowHeight, "Resource/Menu_background.png", renderer);
    gameBackground = new Background(0, 0, windowWidth, windowHeight, "Resource/background.png", renderer);

    // Initialize the buttons
    startButton = new Button(startButtonX, menuButtonsY, buttonWidth, buttonHeight, "Resource/start_button.png", this, Button::ButtonType::START);
    quitButton = new Button(quitButtonX, menuButtonsY, buttonWidth, buttonHeight, "Resource/quit_button.png", this, Button::ButtonType::QUIT);
    goToMenuButton = new Button(centeredX, centeredY, buttonWidth, buttonHeight, "Resource/go_to_menu_button.png", this, Button::ButtonType::GO_TO_MENU);
	scoreButton = new Button(scoreButtonX, menuButtonsY, buttonWidth, buttonHeight, "Resource/highscore_button.png", this, Button::ButtonType::SCORE);
	pauseGoToMenuButton = new Button(startButtonX, menuButtonsY, buttonWidth, buttonHeight, "Resource/go_to_menu_button.png", this, Button::ButtonType::GO_TO_MENU);
	replayButton = new Button(centeredX, menuButtonsY, buttonWidth, buttonHeight, "Resource/replay_button.png", this, Button::ButtonType::START);

    // Initialize font, high score class and score counter
    gameFont = TTF_OpenFont("Resource/ARCADECLASSIC.ttf", 28);
    if (gameFont == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    scoreFont = TTF_OpenFont("Resource/ARCADECLASSIC.ttf", 60);
    if (scoreFont == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    score = new Score(gameFont, renderer);
	highScore = new HighScore("highScore.txt", scoreFont, renderer);
	highScore->load();

    // Initialize player ship and alien
    player = new Player(centeredX, 850, 100, 100, "Resource/player.png", renderer, 400, bullets);
    alienSwarm = new AlienSwarm(renderer, enemiesBullets);
    livesManager = new LivesManager(3, gameFont, renderer);
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        switch (currentState) {
        case GameState::MENU:
            // Handle menu input 
            startButton->handleEvent(event); 
            quitButton->handleEvent(event);  
			scoreButton->handleEvent(event);
            break;
        case GameState::PLAYING:
            // Handle gameplay input 
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                currentState = GameState::PAUSE;
            }
            player->handleEvent(event);
            break;
        case GameState::PAUSE:
            // Handle pause menu input
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                currentState = GameState::PLAYING; // Go back to playing
            }
            goToMenuButton->handleEvent(event);
            break;
        case GameState::GAME_OVER:
            // Handle game over input 
			pauseGoToMenuButton->handleEvent(event);
            quitButton->handleEvent(event);
			replayButton->handleEvent(event);
            break;
		case GameState::HIGHSCORE:
			// Handle highscore input
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                currentState = GameState::MENU;
            }
			break;
        }
    }
}

void Game::update() {
    // Game logic
    Uint32 currentFrameTime = SDL_GetTicks();
    deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Convert to seconds
    switch (currentState) {
    case GameState::MENU:
        // Reset bullets
        for (Bullet* bullet : bullets) {
            delete bullet;
        }
        bullets.clear();

        // Reset enemies bullets
        for (Bullet* bullet : enemiesBullets) {
            delete bullet;
        }
        enemiesBullets.clear();
        alienSwarm->reset(); // This is so that when go from pause to menu then play again, the alien swarm will reset
		alienSwarm->resetResetCounter(); // Reset the reset counter
		alienSwarm->resetNumOfShooter(); // Reset the number of shooter
        score->reset(); // The same for score
		livesManager->reset(); // The same for lives  
        break;
    case GameState::PLAYING: {
        // Update high score flag
		scoreAdded = false;

        // Update player and alien
        player->update(deltaTime);
        alienSwarm->update(deltaTime); 

        // Update player bullets
        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i]->update(deltaTime);
            if (bullets[i]->shouldRemove) {
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
                --i; // Adjust index after erasing
            }
        }

        //Update enemy bullets
        for (size_t i = 0; i < enemiesBullets.size(); ++i) {
            enemiesBullets[i]->update(deltaTime);
            if (enemiesBullets[i]->shouldRemove) {
                delete enemiesBullets[i];
                enemiesBullets.erase(enemiesBullets.begin() + i);
                --i; // Adjust index after erasing
            }
        }

        // Collision detection (bullets and aliens)
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < alienSwarm->getAliens().size(); j++) {
                if (bullets[i]->isColliding(*alienSwarm->getAliens()[j])) {
                    bullets[i]->shouldRemove = true;
                    alienSwarm->getAliens()[j]->shouldRemove = true;
                    score->increaseScore(10);
                    // Increase score, play sound, etc.
                    break; // Break after a collision
                }
            }
        }

        // Collision detection (bullets and player)
        for (int i = 0; i < enemiesBullets.size(); i++) {
            if (enemiesBullets[i]->isColliding(*player)) {
                enemiesBullets[i]->shouldRemove = true;
                livesManager->loseLife();
                // Increase score, play sound, etc.
                break; // Break after a collision
            }
        }

        // Create new alien swarm when previous is defeated
        if (alienSwarm->getAliens().empty())
        {
			alienSwarm->increaseResetCounter(); // Increase the reset counter
            alienSwarm->increaseSpeed(0.1f); // Increase speed
            alienSwarm->reset(); // Spawn new swarm
			alienSwarm->increaseShootingSpeed(0.1f); // Increase shooting speed
			alienSwarm->increaseNumOfShooter(); // Increase number of shooters (if reset counter is 2 and the numOfShooter is less than 15)
        }

        //check if aliens reached the bottom
        for (Alien* alien : alienSwarm->getAliens()) {
            if (alien->getRect().y + alien->getRect().h >= player->getRect().y) {
                setGameState(GameState::GAME_OVER);
                break;
            }
        }

        // Check if player's lives is 0
        if (livesManager->isGameOver())
        {
            setGameState(GameState::GAME_OVER);
        }
        break;
    } 
    case GameState::PAUSE:
        break;
    case GameState::GAME_OVER:
        // Reset bullets
        for (Bullet* bullet : bullets) {
            delete bullet;
        }
        bullets.clear();

        // Reset enemies bullets
        for (Bullet* bullet : enemiesBullets) {
            delete bullet;
        }
        enemiesBullets.clear();
        alienSwarm->reset(); // This is so that when go from pause to menu then play again, the alien swarm will reset
        livesManager->reset(); // The same for lives  
        if (!scoreAdded) {
            highScore->addScore(score->getScore());
            highScore->save();
            scoreAdded = true;
        }
        score->reset();
        break;
    }
    lastFrameTime = currentFrameTime;
}

void Game::render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set clear color to black
    SDL_RenderClear(renderer);

    // Draw game objects 
    switch (currentState) {
    case GameState::MENU:
        // Render menu elements
        startBackground->render(renderer);
        startButton->render(renderer);
		scoreButton->render(renderer);
        quitButton->render(renderer);
        break;
    case GameState::PLAYING:
        // Render game elements 
        gameBackground->render(renderer);
        player->render(renderer);
        alienSwarm->render(renderer); 
        for (Bullet* bullet : bullets) {
            bullet->render(renderer);
        }
        for (Bullet* bullet : enemiesBullets) {
            bullet->render(renderer);
        }
        livesManager->render(10, 10); // Render lives at (10, 10)
        score->render(600, 10);
        break;
    case GameState::PAUSE:
        // Render pause menu
        gameBackground->render(renderer);
        player->render(renderer);
		livesManager->render(10, 10);
        alienSwarm->render(renderer);
        for (Bullet* bullet : bullets) {
            bullet->render(renderer);
        }
        for (Bullet* bullet : enemiesBullets) {
            bullet->render(renderer);
        }
        score->render(600, 10);
        goToMenuButton->render(renderer);
        break;
    case GameState::GAME_OVER:
        // Render game over screen elements
        quitButton->render(renderer);
		pauseGoToMenuButton->render(renderer);
		replayButton->render(renderer);
        break;
	case GameState::HIGHSCORE:
		// Render highscore screen elements
		highScore->render(windowWidth/2, 100, scoreFont);
    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Release font
    if (gameFont)
    {
        TTF_CloseFont(gameFont);
        gameFont = nullptr;
    }
    if (scoreFont)
    {
        TTF_CloseFont(scoreFont);
        scoreFont = nullptr;
    }

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}