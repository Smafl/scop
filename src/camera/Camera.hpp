#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

class Camera {
public:
	std::vector<float> position; // camera position
	std::vector<float> orientation; // camera direction
	std::vector<float> up; // up direction

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 100.0f;

	explicit Camera(int widht, int height, std::vector<float> position);

private:
	Camera();
};
