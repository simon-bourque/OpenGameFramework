#version 430

in vec2 textureCoord;

out vec4 color;

uniform sampler2DArray diffuseTextureAtlas;

uniform int currentFrame;

uniform bool horizontalFlip;
uniform bool verticalFlip;

void main() {
	//color = outColor;
	
	vec4 textureColor = texture(diffuseTextureAtlas, vec3(
		(horizontalFlip) ? (1.0 - textureCoord.x) : textureCoord.x,
		(verticalFlip) ? (1.0 - textureCoord.y) : textureCoord.y,
		currentFrame;
	));
	//if (horizontalFlip) {
		//textureColor = texture(diffuseTextureAtlas, vec3((1.0 - textureCoord.x), textureCoord.y, currentFrame));
	//}
	//else {
		//textureColor = texture(diffuseTextureAtlas, vec3(textureCoord.x, textureCoord.y, currentFrame));
	//}
	
	if (textureColor.a == 0) {
		discard;
	}
	
	color = textureColor;
	//color = vec4(1.0,0.0,0.0,1.0);
}