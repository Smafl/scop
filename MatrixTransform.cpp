#include "MatrixTransform.hpp"
#include <glad/gl.h>
#include <string.h>
#include <math.h>

using namespace std;

MatrixTransformException::MatrixTransformException(ErrorCode err)
	: _errorCode(err) {}

const char *MatrixTransformException::what() const noexcept {
	switch (_errorCode) {
		case EMPTY: return "Matrix is empty";
		default: return "An unknown error occured during matrix transformation";
	}
}

void MatrixTransform::scale(GLfloat *matrix, GLfloat scaleFactor) {
    matrix[0] = matrix[0] * scaleFactor;
    matrix[5] = matrix[5] * scaleFactor;
    matrix[10] = matrix[10] * scaleFactor;
}

void MatrixTransform::rotateX(GLfloat *matrix, GLfloat angleDegree) {
    GLfloat c = cosf(degreesToRadians(angleDegree));
    GLfloat s = sinf(degreesToRadians(angleDegree));

    matrix[5] = c;
    matrix[6] = -s;
    matrix[9] = s;
    matrix[10] = c;
}

void MatrixTransform::rotateY(GLfloat *matrix, GLfloat angleDegree) {
    GLfloat c = cosf(degreesToRadians(angleDegree));
    GLfloat s = sinf(degreesToRadians(angleDegree));

    matrix[0] = c;
    matrix[2] = -s;
    matrix[8] = s;
    matrix[10] = c;
}

void MatrixTransform::rotateZ(GLfloat *matrix, GLfloat angleDegree) {
    GLfloat c = cosf(degreesToRadians(angleDegree));
    GLfloat s = sinf(degreesToRadians(angleDegree));

    matrix[0] = c;
    matrix[1] = -s;
    matrix[4] = s;
    matrix[5] = c;
}

void MatrixTransform::translate(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z) {
	matrix[3] = matrix[3] + x;
    matrix[7] = matrix[7] + y;
    matrix[11] = matrix[11] + z;
}

void MatrixTransform::perspective(GLfloat *matrix, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far) {
    GLfloat f = 1.0f / tan(degreesToRadians(fov) / 2.0f);

    matrix[0] = f / aspect;
    matrix[5] = f;
    matrix[10] = (far+near)/(near-far);
    matrix[11] = -1;
    matrix[14] = (2*far*near)/(near-far);
}

void MatrixTransform::loadIdentity(GLfloat *matrix) {
	for (int i = 0; i < 16; ++i)
        matrix[i] = 0.0f;

	matrix[0] = 1.0f;
	matrix[5] = 1.0f;
	matrix[10] = 1.0f;
	matrix[15] = 1.0f;
}

void MatrixTransform::multiply(GLfloat *m1, GLfloat *m2, GLfloat *result) {
    GLfloat temp[16];
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            GLfloat sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += m1[row * 4 + k] * m2[k * 4 + col];
            }
            temp[row * 4 + col] = sum;
        }
    }
    memcpy(result, temp, sizeof(GLfloat) * 16);
}

GLfloat MatrixTransform::degreesToRadians(GLfloat degrees) {
    return degrees * (3.14159265f / 180.0f);
}
