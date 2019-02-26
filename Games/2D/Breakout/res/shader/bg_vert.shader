#version 430

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 inTextureCoord;

out vec2 textureCoord;

void main() {
	gl_Position = vec4(position, 1.0, 1.0);
	textureCoord = inTextureCoord;
}