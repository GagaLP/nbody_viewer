#pragma once

#include <raylib-cpp.hpp>

enum class LightType { LIGHT_DIRECTIONAL = 0,
                       LIGHT_POINT };

struct Light {
  public:
    int     type;
    bool    enabled;
    Vector3 position;
    Vector3 target;
    Color   color;

    int enabledLoc{};
    int typeLoc{};
    int positionLoc{};
    int targetLoc{};
    int colorLoc{};

    void draw() const;

    void updateLightValues(raylib::Shader& shader);

    void updatePosition(raylib::Shader& shader, raylib::Vector3&& position);

    void defineShader(raylib::Shader& shader);

    Light(LightType type, bool enabled, Vector3&& position, Vector3&& target, Color&& color);
};