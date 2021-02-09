#version 450 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;


void main() {
    gl_Position = projection * (view * vec4(vertex, 1.0));
}

