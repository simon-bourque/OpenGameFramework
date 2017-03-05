#version 430

out vec4 color;

uniform vec4 backgroundColor;

void main() {
	//if (textureColor.a == 0) {
		//discard;
	//}
	
	color = backgroundColor;
}