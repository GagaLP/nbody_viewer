#include "Game.hpp"
#include "Utility.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>

constexpr int GLSL_VERSION = 330;

Game::Game(int width, int height, const std::string& title, int capFPS, std::string shaderPath)
    : window(width, height, title),
      shader("./shader/lighting.vs", "./shader/lighting.fs"),
      light(LightType::LIGHT_POINT, true, { 20, 20, 20 }, raylib::Vector3::Zero(), WHITE) {
	shader.locs[SHADER_LOC_VECTOR_VIEW] = shader.GetLocation("viewPos");

	int ambientLoc = shader.GetLocation("ambient");
	shader.SetValue(ambientLoc, &ambientColor, SHADER_UNIFORM_VEC4);

    initParticles(shaderPath);
    generateSpheres(true);

    float min_x, max_x, min_y, max_y, min_z, max_z;
    std::for_each(particlePositionsFrames[0].begin(), particlePositionsFrames[0].end(), [&](raylib::Vector3& p) mutable {
        min_x = std::min(min_x, p.x);
        max_x = std::max(max_x, p.x);
        min_y = std::min(min_y, p.y);
        max_y = std::max(max_y, p.y);
        min_z = std::min(min_z, p.z);
        max_z = std::max(max_z, p.z);
    });

    std::cout << (max_x - min_x) / 2 << " " << (max_y - min_y) / 2 << " " << (max_z - min_z) / 2 << std::endl;
    raylib::Vector3 target = {(min_x + max_x) / 2 + min_x, (min_y + max_y) / 2 + min_y, (min_z + max_z) / 2 + min_z};
    position = raylib::Vector3(max_x, max_y, max_z);
    camera = raylib::Camera3D(position, target, up, 45.0f, CAMERA_PERSPECTIVE);

    light.position = position;

    light.defineShader(shader);

	camera.SetMode(CAMERA_CUSTOM);

	window.SetTargetFPS(capFPS);
}

Game::~Game() = default;

void Game::generateSpheres(bool single) {
    if (single) {
        spheres.emplace_back(0.1f, RED);
        spheres.back().material.shader = shader;
    } else {
        std::for_each(particlePositionsFrames[0].begin(), particlePositionsFrames[0].end(), [this](raylib::Vector3& position) {
            spheres.emplace_back(0.1f, RED);
            spheres.back().material.shader = shader;
        });
    }
}


void Game::initParticles(std::string shaderPath) {
    std::ifstream file(shaderPath);
    std::string line;
    std::vector<raylib::Vector3> particlePositions;
    while (std::getline(file, line)) {
        if (line.empty()) {
            if (!particlePositions.empty()) {
                particlePositionsFrames.push_back(particlePositions);
            }
            particlePositions.clear();
        } else {
            std::vector<std::string> tokens = Utility::split(line, ' ');
            particlePositions.emplace_back(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]));
        }
    }

    if (particlePositions.empty()) {
        particlePositionsFrames.push_back(particlePositions);
    }
}

// include poll event
void Game::handleInput() {
	static float speed = 1.0f;

	if(IsKeyPressed('P')) {
		debug = !debug;
	}

	if(IsKeyPressed('F')) {
		speed += 0.5f;
	}
	if(IsKeyPressed('R')) {
		speed -= 0.5f;
	}

	if(IsKeyPressed('Q')) {
		paused = !paused;
	}

	if(IsKeyDown('W')) {
		position.y += speed;
	}
	if(IsKeyDown('A')) {
		position.x -= speed;
	}
	if(IsKeyDown('S')) {
		position.y -= speed;
	}
	if(IsKeyDown('D')) {
		position.x += speed;
	}
	if(IsKeyDown('E')) {
		position.z += speed;
	}
	if(IsKeyDown('C')) {
		position.z -= speed;
	}
}

void Game::update() {
	camera.SetPosition(position);
	camera.Update();

	static int x = 0;

    if (!paused) {
        currentFrame = x++ % particlePositionsFrames.size();
    }

	shader.SetValue(shader.locs[SHADER_LOC_VECTOR_VIEW], &position, SHADER_UNIFORM_VEC3);
}

void Game::render() {
	BeginDrawing();
	{
		window.ClearBackground(BLACK);

		if(debug) {
			DrawFPS(10, 10);
		}

		camera.BeginMode();
		{
			if (debug) {
				light.draw();
			}

			for(std::size_t i = 0; i < particlePositionsFrames[currentFrame].size(); ++i) {
				Vector3& p = particlePositionsFrames[currentFrame][i];
//                spheres[i].draw(p);
				spheres[0].draw(p);
			}
		}
		camera.EndMode();
	}
	EndDrawing();
}

bool Game::isRunnign() {
	return !window.ShouldClose() && running;
}

void Game::init() {}