#version 330 core

in vec2 fragTextureUV;
out vec4 FragColor;

uniform sampler2D emissiveMap;


void main()
{
    // Get material emissive color by fetching the texture
    vec4 matEmissive = texture2D(emissiveMap, fragTextureUV.st);

    FragColor = matEmissive;
}
