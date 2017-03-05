#version 430

in vec2 textCoords;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform vec4 textColor;

void main() {

	float alpha = texture(diffuseTexture, textCoords).r;
	
	if (alpha == 0) {
		discard;
	}
	
	color = vec4(textColor.rgb, alpha);
	

}