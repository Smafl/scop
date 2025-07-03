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

void MatrixTransform::loadIdentity(GLfloat *matrix) {
	for (int i = 0; i < 16; ++i)
        matrix[i] = 0.0f;

	matrix[0] = 1.0f;
	matrix[5] = 1.0f;
	matrix[10] = 1.0f;
	matrix[15] = 1.0f;
}

void MatrixTransform::multiply(GLfloat *m1, GLfloat *m2, GLfloat *result) {
	// row- / column- issue
    GLfloat temp[16];
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            GLfloat sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += m1[row + k * 4] * m2[k + col * 4];
            }
            temp[row * 4 + col] = sum;
        }
    }
    memcpy(result, temp, sizeof(GLfloat) * 16);
}

GLfloat MatrixTransform::degreesToRadians(GLfloat degrees) {
    return degrees * (3.14159265f / 180.0f);
}
