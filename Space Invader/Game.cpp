#include "Game.h"
#include <iostream>

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr) {}

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
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { // Or IMG_INIT_JPG, etc.
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 

    isRunning = true;
    lastFrameTime = SDL_GetTicks();
    deltaTime = 0.0f;
    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            break;
        }
    }
}



void Game::update() {
    // Game logic
    Uint32 currentFrameTime = SDL_GetTicks();
    deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Convert to seconds
    lastFrameTime = currentFrameTime;
}

void Game::render() {
    // Clear the screen
    SDL_RenderClear(renderer);

    // Draw game objects 

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