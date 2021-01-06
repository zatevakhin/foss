#version 450 core

uniform vec4 lamp;

out vec4 FragColor;

void main() {
    FragColor = lamp;
}
