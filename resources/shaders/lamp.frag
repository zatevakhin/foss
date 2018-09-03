#version 330 core

uniform vec4 lamp;

out vec4 color;

void main() {

    // Set alle 4 vector values to 1.0f
    color = lamp;

}
