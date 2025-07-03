#pragma once

#include <vector>
#include <glad/gl.h>

class RenderModel {
public:

    // explicit RenderModel(const string &path);
	// RenderModel() will go in private
	RenderModel()
	: _vertices({
    // -0.5f, -0.5f, 0.0f, 1.0f,
    //  0.5f, -0.5f, 0.0f, 1.0f,
    //  0.0f,  0.5f, 0.0f, 1.0f,

    // pyramid big
    // -0.5f, 0.0f, 0.5, 1.0f,   1.0f, 0.0f, 0.0f,
    // -0.5f, 0.0f, -0.5, 1.0f,  0.0f, 1.0f, 0.0f,
    // 0.5f, 0.0f, -0.5f, 1.0f,  0.0f, 0.0f, 1.0f,
    // 0.5f, 0.0f, 0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
    // 0.0f, 0.8f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f, // apex

    //pyramid small
    -0.25f, 0.0f,  0.25f, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.25f, 0.0f, -0.25f, 1.0f,   0.0f, 1.0f, 0.0f,
     0.25f, 0.0f, -0.25f, 1.0f,   0.0f, 0.0f, 1.0f,
     0.25f, 0.0f,  0.25f, 1.0f,   1.0f, 1.0f, 0.0f,
     0.0f,  0.4f,  0.0f,  1.0f,   1.0f, 0.0f, 1.0f  // apex


    // pos(x,y)       // color(r,g,b)
    // -0.5f, -0.5f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,  // vertex 0: red
    //  0.5f, -0.5f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,  // vertex 1: green
    //  0.0f,  0.5f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,  // vertex 2: blue

    // -0.25f, 0.0f, -1.0f,    1.0f, 1.0f, 0.0f,  // vertex 3: yellow
    //  0.25f, 0.0f, -1.0f,    0.0f, 1.0f, 1.0f,  // vertex 4: cyan
    //  0.0f, -0.5f, -1.0f,    1.0f, 0.0f, 1.0f   // vertex 5: magenta

    // cube
    // Front face
    // -0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,  // Bottom-left
    //  0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,  // Bottom-right
    //  0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,  // Top-right
    // -0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,  // Top-left

    // // Back face
    // -0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
    //  0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
    //  0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,
    // -0.5f,  0.5f, -0.5f, 1.0f,   0.5f, 0.5f, 0.5f
	}) {}

    const std::vector<GLfloat> &getVertices() const {
		return _vertices;
	}

private:
    std::vector<GLfloat> _vertices;
};

// check if vertices is empty
