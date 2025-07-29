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
	};

	explicit RenderModelException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};


class RenderModel {
public:

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

    RenderModel();
    void parseFaces(std::istringstream &line);
};
