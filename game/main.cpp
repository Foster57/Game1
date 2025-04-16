// main.cpp
#include "Game.h"
#include "ScoreManager.h"
ScoreManager scoreManager;
int main(int argc, char* argv[]) {
    Game game;

    if (!game.init("Contra Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false)) {
        std::cout << "Failed to initialize game!" << std::endl;
        return -1;
    }

    while (game.running()) {
        game.handleEvents();
        game.update();
        game.render();
        SDL_Delay(16); // Khoảng 60 FPS
    }

    game.clean();
    return 0;
}
