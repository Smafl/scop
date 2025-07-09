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

private:
    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indices;
    std::string _path;

    RenderModel();
};

// check if vertices is empty
