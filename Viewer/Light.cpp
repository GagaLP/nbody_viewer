#include "Light.hpp"

Light::Light(LightType type, bool enabled, Vector3&& position, Vector3&& target, Color&& color,
             float attenuation)
    : type(static_cast<int>(type)), enabled(enabled), position(position), target(target),
      color(color), attenuation(attenuation){};

void Light::updateLightValues(raylib::Shader& shader) {
	shader.SetValue(enabledLoc, &enabled, SHADER_UNIFORM_INT);
	shader.SetValue(typeLoc, &type, SHADER_UNIFORM_INT);

	shader.SetValue(positionLoc, &position, SHADER_UNIFORM_VEC3);
	shader.SetValue(targetLoc, &target, SHADER_UNIFORM_VEC3);

	float colorn[4] = { (float)color.r / (float)255, (float)color.g / (float)255,
		                (float)color.b / (float)255, (float)color.a / (float)255 };

	shader.SetValue(colorLoc, &colorn, SHADER_UNIFORM_VEC4);
}

void Light::defineShader(raylib::Shader& shader) {
	enabledLoc = shader.GetLocation("lights.enabled");
	typeLoc = shader.GetLocation("lights.type");
	positionLoc = shader.GetLocation("lights.position");
	targetLoc = shader.GetLocation("lights.target");
	colorLoc = shader.GetLocation("lights.color");
//	attenuationLoc = shader.GetLocation("lights.attenuation");

	updateLightValues(shader);
}

void Light::draw() const {
	DrawSphere(position, 0.2f, color);

	if (type == static_cast<int>(LightType::LIGHT_DIRECTIONAL)) {
		DrawLine3D(position, target, color);
	}
}
