#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projection;

void main() {
    gl_Position =  projection * mat4(1.0) * mat4(1.0) * vec4(position, 1.0);
}
