#version 430

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 inTextCoords;

uniform mat3 modelMatrix;

out vec2 textCoords;

void main() {
	gl_Position = vec4(modelMatrix * vec3(position, 1.0), 1.0);
	textCoords = inTextCoords;
}