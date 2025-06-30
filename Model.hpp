#pragma once

#include <vector>
#include <glad/gl.h>

class Model {
public:

    // explicit Model(const string &path);
	// Model() will go in private
	Model()
	: _vertices({
    // -0.5f, -0.5f, 0.0f, 1.0f,
    //  0.5f, -0.5f, 0.0f, 1.0f,
    //  0.0f,  0.5f, 0.0f, 1.0f,

    -0.5f, 0.0f, 0.5, 1.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, 0.0f, -0.5, 1.0f,  0.0f, 1.0f, 0.0f,
    0.5f, 0.0f, -0.5f, 1.0f,  0.0f, 0.0f, 1.0f,
    0.5f, 0.0f, 0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
    0.0f, 0.8f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f, // apex

    // pos(x,y)       // color(r,g,b)
    // -0.5f, -0.5f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,  // vertex 0: red
    //  0.5f, -0.5f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,  // vertex 1: green
    //  0.0f,  0.5f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,  // vertex 2: blue

    // -0.25f, 0.0f, -1.0f,    1.0f, 1.0f, 0.0f,  // vertex 3: yellow
    //  0.25f, 0.0f, -1.0f,    0.0f, 1.0f, 1.0f,  // vertex 4: cyan
    //  0.0f, -0.5f, -1.0f,    1.0f, 0.0f, 1.0f   // vertex 5: magenta
	}) {}

    const std::vector<GLfloat> &getVertices() const {
		return _vertices;
	}

private:
    std::vector<GLfloat> _vertices;
};

// check if vertices is empty
