#version 430

layout(location = 0) in vec2 position;

uniform mat3 mvpMatrix;

void main() {
	gl_Position = vec4(mvpMatrix * vec3(position, 1.0), 1.0);
}