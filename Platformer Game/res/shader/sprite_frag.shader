#version 430

in vec2 textureCoord;

out vec4 color;

uniform sampler2D sprite;

uniform bool horizontalFlip;
uniform bool verticalFlip;

void main() {

	vec4 textureColor;
	if (horizontalFlip) {
		if (verticalFlip) {
			textureColor = texture(sprite, vec2((1.0 - textureCoord.x), (1.0 - textureCoord.y)));
		}
		else {
			textureColor = texture(sprite, vec2((1.0 - textureCoord.x), textureCoord.y));
		}
	}
	else {
		if (verticalFlip) {
			textureColor = texture(sprite, vec2(textureCoord.x, (1.0 - textureCoord.y)));
		}
		else {
			textureColor = texture(sprite, vec2(textureCoord.x, textureCoord.y));
		}		
	}
	
	if (textureColor.a == 0) {
		discard;
	}
	
	color = textureColor;
}