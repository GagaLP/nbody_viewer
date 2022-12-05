#include "Game.hpp"
#include <array>
#include <raylib-cpp.hpp>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

int main() {
	Game game(SCREEN_WIDTH, SCREEN_HEIGHT, "N-Body Simulation", 60);

	game.init();
	while(game.isRunnign()) {
		game.handleInput();
		game.update();
		game.render();
	}

	return 0;
}