#include <iostream>
#include <SDL.h>
#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;

    if (!game.init("Space Invader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1710, 980, false)) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    while (game.running()) {
        game.handleEvents();
        game.update();
        game.render();
    }

    game.clean();
    return 0;
}