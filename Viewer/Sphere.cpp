#include "Sphere.hpp"

Sphere::Sphere(float radius, raylib::Color color) : mesh(raylib::Mesh::Sphere(radius, 8, 10)), material() {
	material.maps[MATERIAL_MAP_DIFFUSE].color = color;
}

void Sphere::draw(const raylib::Vector3& position) {
	DrawMesh(mesh, material, MatrixMultiply(MatrixIdentity(), MatrixTranslate(position.x, position.y, position.z)));
}

Sphere::Sphere(Sphere&& sphere) : mesh(std::move(sphere.mesh)), material(std::move(sphere.material)) {}
