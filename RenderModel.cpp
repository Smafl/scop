#include "RenderModel.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/gl.h>

using namespace std;

RenderModelException::RenderModelException(ErrorCode err)
	: _errorCode(err) {}

const char *RenderModelException::what() const noexcept {
	switch (_errorCode) {
		case FILE_NOT_FOUND: return "File with rendering model not found";
		case CANNOT_OPEN: return "File with rendering model cannot be opened";
		default: return "An unknown error occured during rendering model creating";
	}
}

RenderModel::RenderModel(const string &path) :
	_path(path)
{
	if (_path.empty()) {
		throw RenderModelException(RenderModelException::FILE_NOT_FOUND);
	}

	ifstream file;
	file.open(_path);
	if (!file.is_open()) {
		throw RenderModelException(RenderModelException::CANNOT_OPEN);
	}
	string line;
	string temp;
	string temp2;
	string temp3;
	int indFlag = 1;
	while (getline(file, line)) {
		if (line[0] == 'v' && line[1] == ' ') {
			line.erase(0, 1);
			istringstream sline(line);
			sline >> temp;
			sline >> temp2;
			sline >> temp3;
			_vertices.push_back(stof(temp));
			_vertices.push_back(stof(temp2));
			_vertices.push_back(stof(temp3));
			_vertices.push_back(1.0);
		} else if (line[0] == 'f' && line[1] == ' ') {
			line.erase(0, 1);
			istringstream sline(line);
			sline >> temp;
			sline >> temp2;
			sline >> temp3;
			if (stoul(temp) == 0 || stoul(temp) == 0 || stoul(temp) == 0) {
				indFlag = 0;
			}
			_indices.push_back(stoul(temp));
			_indices.push_back(stoul(temp2));
			_indices.push_back(stoul(temp3));
		}
	}
	file.close();

	if (indFlag == 1) {
		for (auto& i : _indices) {
			i = i - 1;
		}
	}
}

const vector<GLfloat> &RenderModel::getVertices() const {
	return _vertices;
}

const vector<GLuint> &RenderModel::getIndices() const {
	return _indices;
}
