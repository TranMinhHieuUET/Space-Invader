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

// Spacing between buttons
static int spacing = 50;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), lastFrameTime(0), deltaTime(0.0f), currentState(GameState::MENU) {}

Game::~Game() {
    delete startButton;
    delete quitButton;
    delete goToMenuButton;
    delete startBackground;
    delete gameBackground;
    delete player;
    delete alienSwarm;
    for (Bullet* bullet : bullets) {
        delete bullet;
    }
    delete score;
    if (gameFont)
    {
        TTF_CloseFont(gameFont);
    }
    bullets.clear();
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
    // Get window dimensions
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate button positions
    int centeredX = (windowWidth - buttonWidth) / 2;
    int startButtonY = (windowHeight / 2);
    int quitButtonY = (windowHeight / 2) + buttonHeight + spacing;
    int goToMenuButtonY = (windowHeight / 2);

    // Initialize the background
    startBackground = new Background(0, 0, windowWidth, windowHeight, "Resource/Menu_background.png", renderer);
    gameBackground = new Background(0, 0, windowWidth, windowHeight, "Resource/background.png", renderer);

    // Initialize the buttons
    startButton = new Button(centeredX, startButtonY, buttonWidth, buttonHeight, "Resource/start_button.png", this, Button::ButtonType::START);
    quitButton = new Button(centeredX, quitButtonY, buttonWidth, buttonHeight, "Resource/quit_button.png", this, Button::ButtonType::QUIT);
    goToMenuButton = new Button(centeredX, goToMenuButtonY, buttonWidth, buttonHeight, "Resource/go_to_menu_button.png", this, Button::ButtonType::GO_TO_MENU);

    // Initialize font
    gameFont = TTF_OpenFont("Resource/ARCADECLASSIC.ttf", 28);
    score = new Score(gameFont, renderer);

    // Initialize player ship and alien
    player = new Player(centeredX, 850, 100, 100, "Resource/player.png", renderer, 400, bullets);
    alienSwarm = new AlienSwarm(renderer);
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
            goToMenuButton->handleEvent(event);
            break;
        case GameState::GAME_OVER:
            // Handle game over input 
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
        alienSwarm->reset(); // This is so that when go from pause to menu then play again, the alien swarm will reset
        score->reset(); // The same for score
        break;
    case GameState::PLAYING: {
        // Update player and alien
        player->update(deltaTime);
        alienSwarm->update(deltaTime); 

        // Update bullets
        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i]->update(deltaTime);
            if (bullets[i]->shouldRemove) {
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
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
                    break; // Important: Break after a collision
                }
            }
        }

        // Create new alien swarm when previous is defeated
        if (alienSwarm->getAliens().empty())
        {
            alienSwarm->increaseSpeed(0.1f); // Increase speed
            alienSwarm->reset(); // Spawn new swarm
        }

        //check if aliens reached the bottom
        for (Alien* alien : alienSwarm->getAliens()) {
            if (alien->getRect().y + alien->getRect().h >= player->getRect().y) {
                setGameState(GameState::GAME_OVER);
                alienSwarm->reset();
                break;
            }
        }
        break;
    } 
    case GameState::PAUSE:
        break;
    case GameState::GAME_OVER:
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
        score->render(600, 10);
        break;
    case GameState::PAUSE:
        // Render pause menu
        goToMenuButton->render(renderer);
        break;
    case GameState::GAME_OVER:
        // Render game over screen elements
        break;
    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}