#version 430

in vec2 textureCoord;

out vec4 color;

uniform sampler2D diffuseTexture;

void main() {
	
	vec4 textureColor = texture(diffuseTexture, textureCoord);
	
	if (textureColor.a == 0) {
		discard;
	}
	
	color = textureColor;
}