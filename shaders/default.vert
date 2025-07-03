#version 410 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 modelMatrix;

void main() {
	gl_Position = modelMatrix * vec4(aPos);
	vertexColor = aColor;
}