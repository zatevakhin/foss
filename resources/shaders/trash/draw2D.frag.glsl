#version 330 core

out vec4 color;

uniform vec4 background;

void main() {
    color = background; //vec4(0.2f, 0.2f, 0.2f, 0.5f);
}