#pragma once

#include <fstream>
#include <random>
#include <raylib-cpp.hpp>
#include "Sphere.hpp"

#include "Light.hpp"
#include "OrbitalCamera.hpp"

class Viewer {
  private:
    raylib::Window window;
    OrbitalCamera  camera  = {};
    bool           running = true;
    bool           debug   = true;
    bool           paused  = true;

    raylib::Vector3 position = {20.0f, 20.0f, 20.0f};

    std::vector<Sphere> spheres;

    raylib::Vector4 ambientColor = {0.5f, 0.5f, 0.5f, 1.0f};

    Light            light;
    raylib::Shader   shader;
    raylib::Material material;

    std::vector<std::vector<raylib::Matrix>> particlePositionsFrames;
    int                                      currentFrame = 0;

  public:
    Viewer(int width, int height, const std::string& title, int capFPS);
    ~Viewer();

    void initParticles(std::ifstream& sstream);
    void generateSpheres();

    void handleInput();
    void update();
    void render();
    bool isRunnign();
};
