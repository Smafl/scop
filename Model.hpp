#pragma once

#include <vector>
#include <glad/gl.h>

using namespace std;

class Model {
public:

    // explicit Model(const string &path);
	// Model() will go in private
	Model()
	: _vertices({
		-0.5, -0.5,   // 0 - Lower left
		0.5, -0.5,   // 1 - Lower right
		0.0,  0.5, // 2 - Top center
	}) {}

    const vector<GLfloat> &getVertices() const {
		return _vertices;
	}

private:
    vector<GLfloat> _vertices;
};

// check if vertices is empty
