#include "Game.h"
#include <iostream>
#include "Utils.h"

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
            handleMenuEvents(event);
            break;
        case GameState::PLAYING:
            // Handle gameplay input 
            break;
        case GameState::PAUSE:
            // Handle pause menu input
            break;
        case GameState::GAME_OVER:
            // Handle game over input 
            break;
        }
    }
}

void Game::handleMenuEvents(const SDL_Event& event) {
    // Handle mouse click event
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
    }
}

void Game::renderGameMenu() {
    // Render the menu background, title, and buttons
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
    SDL_RenderClear(renderer);

    // Draw game objects 
    switch (currentState) {
    case GameState::MENU:
        // Render menu elements
        renderGameMenu(); // We'll create this function later
        break;
    case GameState::PLAYING:
        // Render game elements 
        break;
    case GameState::PAUSE:
        // Render pause menu
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
    SDL_Quit();
}