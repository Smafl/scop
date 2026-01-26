#include "Transformation.hpp"
#include "../matrixMath/MatrixTransform.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

Transformation::Transformation() {

}

void Transformation::updateModelMatrix() {
	MatrixTransform::loadIdentity(scale);
	MatrixTransform::scale(scale, transform.scaleFactor);

	MatrixTransform::loadIdentity(rotateY);
	MatrixTransform::rotateY(rotateY, animationState.rotation);
	MatrixTransform::multiply(rotateY, scale, modelMatrix);

	MatrixTransform::translate(modelMatrix, transform.translationX, transform.translationY, transform.translationZ);
}

