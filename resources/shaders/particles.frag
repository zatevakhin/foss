#version 330 core

in vec2 fragTextureUV;
in float alpha_value;

out vec4 FragColor;

uniform sampler2D emissiveMap;


void main()
{
    // Get material emissive color by fetching the texture
    vec4 matEmissive = texture2D(emissiveMap, fragTextureUV.st);

    FragColor = matEmissive * vec4(vec3(1), alpha_value);
}
