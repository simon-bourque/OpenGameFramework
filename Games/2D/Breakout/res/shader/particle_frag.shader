#version 430

in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 color;

uniform sampler2D tex;

void main()
{
    color = (texture(tex, TexCoords) * ParticleColor);
}  