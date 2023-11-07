#include "Viewer.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include <Functions.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <thread>
#include "Utility.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "dark/style_dark.h"

Viewer::Viewer(int width, int height, const std::string& title, int capFPS)
    : window(width, height, title),
      light(LightType::LIGHT_POINT, true, {20, 20, 20}, raylib::Vector3::Zero(), raylib::Color::White()),
      shader("resources/lighting.vs", "resources/lighting.fs") {
    GuiLoadStyleDark();

    shader.locs[SHADER_LOC_MATRIX_MVP]   = GetShaderLocation(shader, "mvp");
    shader.locs[SHADER_LOC_VECTOR_VIEW]  = GetShaderLocation(shader, "viewPos");
    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instanceTransform");

    int ambientLoc = shader.GetLocation("ambient");

    shader.SetValue(ambientLoc, &ambientColor, SHADER_UNIFORM_VEC4);

    generateSpheres();

    light.defineShader(shader);

    window.SetTargetFPS(capFPS);
}

Viewer::~Viewer() = default;

void Viewer::generateSpheres() {
    spheres.emplace_back(0.1f, RED);
    spheres.back().material.shader = shader;
}

void Viewer::initParticles(std::ifstream& sstream) {
    std::string line;

    std::vector<raylib::Matrix> particlePositions;
    particlePositionsFrames.clear();
    while (std::getline(sstream, line)) {
        if (line.empty()) {
            if (!particlePositions.empty()) {
                particlePositionsFrames.push_back(std::move(particlePositions));
            }
            particlePositions.clear();
        } else {
            std::vector<std::string> tokens = Utility::split(line, ' ');
            particlePositions.push_back(raylib::Matrix::Translate(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2])));
        }
    }

    if (particlePositions.empty()) {
        particlePositionsFrames.push_back(std::move(particlePositions));
    }

    // print particle positions
    float min_x, max_x, min_y, max_y, min_z, max_z;

    min_x = min_y = min_z = std::numeric_limits<float>::max();
    max_x = max_y = max_z = std::numeric_limits<float>::min();

    std::for_each(particlePositionsFrames[0].begin(), particlePositionsFrames[0].end(), [&](raylib::Matrix& p) mutable {
        min_x = std::min(min_x, p.m12);
        max_x = std::max(max_x, p.m12);
        min_y = std::min(min_y, p.m13);
        max_y = std::max(max_y, p.m13);
        min_z = std::min(min_z, p.m14);
        max_z = std::max(max_z, p.m14);
    });

    raylib::Vector3 target = {(max_x - min_x) / 2 + min_x, (max_y - min_y) / 2 + min_y, (max_z - min_z) / 2 + min_z};

    position = raylib::Vector3(max_x, max_y, max_z);
    camera   = {position, target};

    light.position = position;
}

// include poll event
void Viewer::handleInput() {
    camera.handleInput();

    if (IsKeyPressed('P')) {
        debug = !debug;
    }

    if (IsKeyPressed('F')) {
    }
    if (IsKeyPressed('R')) {
    }

    if (IsKeyPressed('Q')) {
        paused = !paused;
    }

    if (IsKeyPressed('I')) {
        spheres[0].updateRadius(0.001f);
    }

    if (IsKeyPressed('L')) {
        spheres[0].updateRadius(0.01f);
    }

    if (IsKeyPressed('O')) {
        spheres[0].updateRadius(0.2f);
    }

    if (IsKeyPressed('K')) {
        spheres[0].updateRadius(0.1f);
    }
}

void Viewer::update() {
    camera.update();

    static int x = 0;
    static int y = 0;
    if (!paused && y >= 0) {
        currentFrame = x++ % particlePositionsFrames.size();
        y            = 0;
    } else if (!paused) {
        y++;
    }

    shader.SetValue(shader.locs[SHADER_LOC_VECTOR_VIEW], &position, SHADER_UNIFORM_VEC3);

    if (IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles();

        std::ifstream sstream(droppedFiles.paths[droppedFiles.count - 1]);
        initParticles(sstream);
        generateSpheres();

        UnloadDroppedFiles(droppedFiles);
    }
}

void Viewer::render() {
    BeginDrawing();
    {
        window.ClearBackground(BLACK);

        if (debug) {
            DrawFPS(10, 10);
        }

        camera.BeginMode();
        {
            if (debug) {
                light.draw();
            }

            DrawMeshInstanced(spheres[0].mesh, spheres[0].material, particlePositionsFrames[currentFrame].data(), particlePositionsFrames[currentFrame].size());
        }
        camera.EndMode();

        // gui button
        // if (GuiButton({96, 96, 216, 32}, "Open file")) {
        //     DrawText("Drop new files...", 100, 110 + 40, 20, DARKGRAY);
        // }

        // gui window box
        // Rectangle windowBoxRect = {10, 10, 200, 300};

        // if (debug) {
        //     debug = !GuiWindowBox(windowBoxRect, "Window Box");

        //     if (GuiButton({20, 50, 180, 40}, "Button")) {
        //         // do something when the button is pressed
        //     }

        //     // add slider to the gui window box
        //     static float value = 0;
        //     if (GuiSlider({20, 100, 180, 20}, NULL, NULL, &value, 0.0f, 1.0f)) {
        //         spheres[0].updateRadius(value);
        //     }
        // }
    }
    EndDrawing();
}

bool Viewer::isRunnign() {
    return !window.ShouldClose() && running;
}
