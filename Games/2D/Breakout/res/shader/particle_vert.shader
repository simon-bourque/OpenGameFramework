#version 430

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform vec2 offset;
uniform vec4 color;
uniform mat3 vpMatrix;

void main()
{
    float scale = 1.0f;
    TexCoords = texCoords.xy;
    ParticleColor = color;
    gl_Position = mat4(vpMatrix) * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}