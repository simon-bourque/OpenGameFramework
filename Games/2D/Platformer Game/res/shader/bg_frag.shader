#version 430

in vec2 textureCoord;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform float xOffset;

void main() {
	vec4 textureColor = texture(diffuseTexture, vec2(textureCoord.x + xOffset, textureCoord.y));
	
	if (textureColor.a == 0) {
		discard;
	}
	
	color = textureColor;
}