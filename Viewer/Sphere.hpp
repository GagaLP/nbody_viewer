#pragma once

#include <raylib-cpp.hpp>

class Sphere {
	raylib::Mesh mesh;

  public:
	raylib::Material material;

	Sphere(float radius, raylib::Color color);

	Sphere(Sphere&& sphere);
//	Sphere(const raylib::Mesh& mesh, const raylib::Material& material);
	~Sphere() = default;

	void draw(const raylib::Vector3& position);
};

