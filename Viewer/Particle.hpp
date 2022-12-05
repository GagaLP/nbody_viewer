#pragma once

#include "Vector3D.hpp"

struct Particle {
	Vector3D position;
	Vector3D velocity;
	Vector3D force;
	float mass;
	float radius;

	Particle(Vector3D position, Vector3D velocity, float mass, float radius)
	    : position(position), velocity(velocity), mass(mass), radius(radius) {}

	Particle(Vector3D position, float mass, float radius)
	    : position(position), velocity(), mass(mass), radius(radius) {}

	~Particle() = default;

	Particle() : position(), velocity(), mass(0), radius(0) {}

	Particle& operator=(const Particle& particle) {
		position = particle.position;
		velocity = particle.velocity;
		mass = particle.mass;
		radius = particle.radius;
		return *this;
	}

	bool operator==(const Particle& particle) const {
		return position == particle.position && velocity == particle.velocity &&
		       mass == particle.mass && radius == particle.radius;
	}
};
