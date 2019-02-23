#version 430

in vec2 textureCoord;
flat in int index;

out vec4 color;

uniform sampler2DArray tilesheet;

void main() {
	vec4 textureColor = texture(tilesheet, vec3(textureCoord.x, textureCoord.y, index));
	
	if (textureColor.a == 0) {
		discard;
	}
	
	color = textureColor;
	
}