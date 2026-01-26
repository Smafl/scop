#include "Camera.hpp"
#include "../matrixMath/MatrixTransform.hpp"

Camera::Camera(GLfloat width, GLfloat height)
	: width(width), height(height)
{
	aspect = width / height;
	updateView();
	updateProjection();
}

void Camera::updateProjection() {
    MatrixTransform::loadIdentity(projectionMatrix);
    MatrixTransform::perspective(projectionMatrix, fov, aspect, near, far);
}

void Camera::updateView() {
	MatrixTransform::loadIdentity(viewMatrix);
	MatrixTransform::translate(viewMatrix, 0.0f, 0.0f, 0.0f);
}
