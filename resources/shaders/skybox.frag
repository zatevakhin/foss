#version 450 core

in vec3 TexCoords;
out vec4 color;

uniform samplerCube cubemap;

void main() {
    color = texture(cubemap, TexCoords);
}
