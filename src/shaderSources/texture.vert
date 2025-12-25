#version 410 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main() {
	gl_Position = projectionMatrix * modelMatrix * vec4(aPos);
	TexCoord = aTexCoord;
}