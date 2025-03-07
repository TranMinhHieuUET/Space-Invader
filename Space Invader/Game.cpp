#include "Game.h"
#include <iostream>
#include "Utils.h"
#include "Button.h"

// Button dimensions
static int buttonWidth = 200;
static int buttonHeight = 100;

// Spacing between buttons
static int spacing = 50;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), lastFrameTime(0), deltaTime(0.0f), currentState(GameState::MENU) {}

Game::~Game() {}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
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

    //Initialize SDL_Image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { 
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 

    // Render the start menu
    renderButton();

    isRunning = true;
    lastFrameTime = SDL_GetTicks();
    deltaTime = 0.0f;

    return true;
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
                isRunning = false;
            }
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

void Game::renderButton() {
    // Get window dimensions
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate button positions
    int startButtonX = (windowWidth - buttonWidth) / 2; 
    int startButtonY = (windowHeight / 2);
    int quitButtonX = (windowWidth - buttonWidth) / 2; 
    int quitButtonY = (windowHeight / 2) + buttonHeight + spacing;
    int goToMenuButtonX = (windowWidth - buttonWidth) / 2;
    int goToMenuButtonY = (windowHeight / 2);

    // Initialize the Start button and Quit button
    startButton = std::make_unique<Button>(startButtonX, startButtonY, buttonWidth, buttonHeight, "Resource/start_button.png", this, Button::ButtonType::START);
    quitButton = std::make_unique<Button>(quitButtonX, quitButtonY, buttonWidth, buttonHeight, "Resource/quit_button.png", this, Button::ButtonType::QUIT);
    goToMenuButton = std::make_unique<Button>(goToMenuButtonX, goToMenuButtonY, buttonWidth, buttonHeight, "Resource/go_to_menu_button.png", this, Button::ButtonType::GO_TO_MENU);

}

void Game::update() {
    // Game logic
    Uint32 currentFrameTime = SDL_GetTicks();
    switch (currentState) {
    case GameState::MENU:
        break;
    case GameState::PLAYING: {
        break;
    } 
    case GameState::PAUSE:
        break;
    case GameState::GAME_OVER:
        break;
    }
    deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Convert to seconds
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
        startButton->render(renderer);
        quitButton->render(renderer);
        break;
    case GameState::PLAYING:
        // Render game elements 
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
    // Cleanup buttons
    startButton.reset();
    quitButton.reset();

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
}