#version 430

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 inTextureCoord;
layout(location = 2) in int inIndex;
layout(location = 3) in mat3 modelMatrix;

out vec2 textureCoord;
flat out int index;

uniform mat3 vpMatrix;

void main() {
	gl_Position = vec4(vpMatrix * modelMatrix * vec3(position, 1.0) , 1.0);
	//gl_Position = vpMatrix * modelMatrix * position;
	textureCoord = inTextureCoord;
	index = inIndex;
}