#pragma once

#include <raylib-cpp.hpp>

class Sphere {
  public:
    raylib::Mesh     mesh;
    raylib::Material material;

    Sphere(float radius, raylib::Color color);

    Sphere(Sphere&& sphere);

    ~Sphere() = default;

    void updateRadius(const float radius);

    void draw(const raylib::Vector3& position);
};
