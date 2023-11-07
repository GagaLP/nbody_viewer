#include <raylib-cpp.hpp>
#include "Utility.hpp"

class OrbitalCamera {
  public:
    OrbitalCamera()
        : camera() {}

    OrbitalCamera(raylib::Vector3 position, raylib::Vector3 target)
        : camera(position, target, {0.0f, 1.0f, 0.0f}, 45.0f, CAMERA_PERSPECTIVE) {}

    void handleInput() {
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            tiltAngle += 5.0f * rotSpeed;
        }

        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            tiltAngle -= 5.0f * rotSpeed;
        }

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            rotAngle += 5.0f * rotSpeed;
        }

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            rotAngle -= 5.0f * rotSpeed;
        }

        // update zoom
        camDist += GetMouseWheelMove();
        if (camDist < 0) {
            camDist = 0;
        }
    }

    void update() {
        tiltAngle = Utility::clamp(tiltAngle, -90.0f, 90.0f);

        Vector3 camPos = {0, 0, camDist};

        Matrix tiltMat = MatrixRotateX(tiltAngle * GetFrameTime());
        Matrix rotMat  = MatrixRotateY(rotAngle * GetFrameTime());
        Matrix mat     = MatrixMultiply(tiltMat, rotMat);

        camPos = Vector3Transform(camPos, mat);

        camera.position = Vector3Add(camera.target, camPos);
    }

    void BeginMode() {
        camera.BeginMode();
    }

    void EndMode() {
        camera.EndMode();
    }

  private:
    raylib::Camera3D camera;

    float camDist   = 10;
    float rotAngle  = 45;
    float tiltAngle = 45;

    float rotSpeed = 0.25f;
};
