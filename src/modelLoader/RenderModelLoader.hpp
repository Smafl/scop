
/**
* @file RenderModelLoader.hpp
* @brief Minimal Wavefront OBJ loader for OpenGL rendering.
*
* This module parses geometry data from an OBJ file and prepares
* vertex and index buffers suitable for OpenGL rendering.
*
* Supported OBJ elements:
*  - v   (vertex positions)
*  - vt  (texture coordinates)
*  - vn  (normal vectors)
*  - f   (faces, triangulated using triangle fan)
*
* Unsupported / ignored:
*  - Materials (.mtl)
*  - Multiple objects / groups
*  - Negative indices
*/

#pragma once

#include <vector>
#include <string>
#include <exception>
#include <glad/gl.h>
#include <sstream>

/**
* @class RenderModelLoaderException
* @brief Exception type used for OBJ model loading errors.
*
* Provides categorized error reporting for file access
* and parsing failures.
*/
class RenderModelLoaderException : public std::exception {
public:
	enum ErrorCode {
		FILE_NOT_FOUND,
        CANNOT_OPEN,
        INVALID_FACE_FORMAT,
		INDEX_OUT_OF_RANGE,
        UNKNOWN_ERROR,
	};

	explicit RenderModelLoaderException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};


/**
* @class RenderModelLoader
* @brief Represents a renderable 3D model loaded from an OBJ file.
*
* The class loads vertex data, texture coordinates, normals,
* and face indices from a Wavefront OBJ file.
*
* Faces with more than three vertices are automatically
* triangulated using a triangle fan approach.
*
* Vertex data is stored separately from index buffers,
* enabling indexed rendering (e.g. via OpenGL EBOs).
*/
class RenderModelLoader {
public:
    GLfloat translationX = 0.0f;
    GLfloat translationY = 0.0f;
    GLfloat translationZ = 0.0f;
    GLfloat delta = 0.05f;
    GLfloat rotation = 0.0f;
    bool isRotate = false;
    GLfloat minZ = -5.0f;
    GLfloat maxZ = 0.5f;
    GLfloat scaleFactor = 0.075f;

    explicit RenderModelLoader(const std::string &path);

    const std::vector<GLfloat> &getFinalVertices() const;
    const std::vector<GLuint> &getFinalIndices() const;

    const std::vector<GLfloat> &getVertices() const;
    const std::vector<GLuint> &getTextures() const;
    const std::vector<GLuint> &getNormals() const;

    const std::vector<GLuint> &getVIndices() const;
    const std::vector<GLuint> &getVtIndices() const;
    const std::vector<GLuint> &getVnIndices() const;

private:
    std::vector<GLfloat> _finalVertices; // not implemented
    std::vector<GLuint>  _finalIndices; // not implemented

    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _texture;
    std::vector<GLuint> _normals;

    std::vector<GLuint> _vIndices;
    std::vector<GLuint> _vtIndices;
    std::vector<GLuint> _vnIndices;

    std::string _path;

    void parseFaces(std::istringstream &line);
    RenderModelLoader();
};
