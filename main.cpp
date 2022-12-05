#include "./Viewer/Game.hpp"

#include <array>
#include <raylib-cpp.hpp>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

int main(int argc, char** argv) {
    std::string filename = "output.txt";
    if (argc >= 2) {
        filename = argv[1];
    }

    Game game(SCREEN_WIDTH, SCREEN_HEIGHT, "N-Body Simulation", 60, filename);

    game.init();
    while(game.isRunnign()) {
        game.handleInput();
        game.update();
        game.render();
    }

    return 0;
}