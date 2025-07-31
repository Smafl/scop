#pragma once

#include <vector>
#include <string>
#include <exception>
#include <glad/gl.h>

class RenderModelException : public std::exception {
public:
	enum ErrorCode {
		FILE_NOT_FOUND,
        CANNOT_OPEN,
        INVALID_FACE_FORMAT,
		INDEX_OUT_OF_RANGE,
        UNKNOWN_ERROR,
	};

	explicit RenderModelException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};


class RenderModel {
public:
    GLfloat translationX = 0.0f;
    GLfloat translationY = 0.0f;
    GLfloat translationZ = 0.0f;
    GLfloat delta = 0.05f;
    GLfloat rotation = 0.0f;
    bool isRotate = true;
    GLfloat minZ = -5.0f;
    GLfloat maxZ = 0.5f;

    explicit RenderModel(const std::string &path);

    const std::vector<GLfloat> &getVertices() const;
    const std::vector<GLuint> &getIndices() const;
    const std::vector<GLuint> &getTextures() const;
    const std::vector<GLuint> &getNormals() const;

private:
    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indices;
    std::vector<GLuint> _textures;
    std::vector<GLuint> _normals;
    std::string _path;

    void parseFaces(std::istringstream &line);
    RenderModel();
};
