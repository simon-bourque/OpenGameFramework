#version 430

layout(location = 0) in vec4 position;

uniform mat4 modelMatrix;

void main() {
	gl_Position = modelMatrix * position;
}