#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int frameNumber;
uniform sampler2D screenTexture;


float rand(float x, float y)
{
    return fract(sin(dot(vec2(x, y), vec2(12.9898, 78.233))) * 43758.5453);
}

float nosie(float a, float b, float intensity)
{
    return rand(a, b) * intensity;
}

void main()
{
    vec4 color = texture(screenTexture, TexCoords);

    color += nosie(0, TexCoords.y + sin(frameNumber), 0.05);
    color += nosie(TexCoords.x + sin(frameNumber), TexCoords.y + sin(frameNumber), 0.07);

    FragColor = color;
}
