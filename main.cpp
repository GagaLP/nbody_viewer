#include "./Viewer/Viewer.hpp"

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <raylib-cpp.hpp>
#include <sstream>
#include <thread>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#if defined(PLATFORM_WEB)
constexpr int SCREEN_WIDTH  = 1280;
constexpr int SCREEN_HEIGHT = 720;
#else
constexpr int SCREEN_WIDTH  = 0;
constexpr int SCREEN_HEIGHT = 0;
#endif

// because emscripten_set_main_loop() requires a function without parameters
Viewer viewer(SCREEN_WIDTH, SCREEN_HEIGHT, "N-Body Simulation", 60);

void drawloop(void) {
    viewer.handleInput();
    viewer.update();
    viewer.render();
}

bool run = false;

int main(int argc, char** argv) {
#if defined(PLATFORM_WEB)
    EM_ASM({ document.getElementById("canvas").style.width = "100%"; });
#endif

    std::string filename = "resources/output.txt";
    if (argc >= 2) {
        filename = argv[1];
    }

    // std::istringstream sstream = std::istringstream(raylib::LoadFileText(filename));
    std::ifstream sstream(filename);
    viewer.initParticles(sstream);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(drawloop, 0, 1);
#else
    while (viewer.isRunnign()) {
        drawloop();
    }
#endif

    return 0;
}