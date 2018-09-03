#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{

    // color = vec4(texture(texture_diffuse1, TexCoords));
        color = texture(texture_diffuse1, TexCoords) * color;
//    color = mix(texture(texture_diffuse1, TexCoords), texture(texture_diffuse1, vec2(TexCoords.x, 1.0f - TexCoords.y)), 0.1) * color;

}