#pragma once

#include "../matrixMath/MatrixTransform.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>


struct AnimationState {
    GLfloat delta = 0.05f;
    GLfloat rotation = 0.0f;
    bool isRotate = false;
};

struct Transform {
	GLfloat translationX = 0.0f;
    GLfloat translationY = 0.0f;
    GLfloat translationZ = 0.0f;

	GLfloat rotationX = 0.0f;
	GLfloat rotationY = 0.0f;
	GLfloat rotationZ = 0.0f;

    GLfloat scaleFactor = 0.075f;
};

class Transformation {
public:
	AnimationState animationState;
	Transform transform;

	GLfloat modelMatrix[16];
	GLfloat scale[16], rotateX[16], rotateY[16], rotateZ[16], temp[16];

	GLfloat minZ = -5.0f;
    GLfloat maxZ = 0.5f;

	Transformation();

	void updateModelMatrix();

private:
};
