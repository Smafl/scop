#pragma once

#include <exception>
#include <vector>
#include <string>
#include <sstream>
#include <glad/gl.h>

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


struct Vertex {
    GLfloat x, y, z;
};

struct TexCoord {
    GLfloat u, v;
};

struct Normal {
    GLfloat x, y, z;
};

struct FaceVertex {
    GLint vertexIndex;
    GLint texCoordIndex;
    GLint normalIndex;
};

struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> texCoords;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> colors;
    std::vector<GLuint> indices;
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
    explicit RenderModelLoader(const std::string &path);

    const Mesh &getMesh() const;

private:
    Mesh _mesh;

    std::vector<Vertex> _vertex;
    std::vector<TexCoord> _texcoord;
    std::vector<Normal> _normal;
    std::vector<std::vector<FaceVertex>> _faces;

    std::string _path;

    void parseFaces(std::istringstream &line);
    void buildMesh();
    RenderModelLoader();
};
