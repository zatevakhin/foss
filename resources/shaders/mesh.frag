#version 450 core

out vec4 FragColor;
in vec2 fragTextureUV;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


void main()
{
    // Get material emissive color by fetching the texture
    vec4 matEmissive = texture(texture_diffuse1, fragTextureUV.st);

    FragColor = matEmissive;
}
