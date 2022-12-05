#pragma once

#include <raylib-cpp.hpp>

enum class LightType { LIGHT_DIRECTIONAL = 0, LIGHT_POINT };

struct Light {
  public:
	int type;
	bool enabled;
	Vector3 position;
	Vector3 target;
	Color color;
	float attenuation;

	int enabledLoc{};
	int typeLoc{};
	int positionLoc{};
	int targetLoc{};
	int colorLoc{};
	int attenuationLoc{};

	void draw() const;

	void updateLightValues(raylib::Shader& shader);

	void defineShader(raylib::Shader& shader);

	Light(LightType type, bool enabled, Vector3&& position, Vector3&& target, Color&& color, float attenuation);
};