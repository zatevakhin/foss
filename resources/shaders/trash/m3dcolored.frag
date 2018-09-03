#version 330 core

out vec4 color;

uniform vec4 background;

void main() {

    color = background;
    // color = vec4(texture(texture_diffuse1, TexCoords));
//        color = texture(texture_diffuse1, TexCoords) * color;
//    color = mix(texture(texture_diffuse1, TexCoords), texture(texture_diffuse1, vec2(TexCoords.x, 1.0f - TexCoords.y)), 0.1) * color;

}