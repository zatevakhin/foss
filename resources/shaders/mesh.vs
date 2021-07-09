#version 450 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 FragmentPosition;
out vec3 Normal;
out vec2 FragTextureUV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main() {
    Normal = mat3(transpose(inverse(model))) * normal;
    FragmentPosition = vec3(model * vec4(vertex, 1.0));
    FragTextureUV = uv;

    gl_Position = projection * (view * vec4(FragmentPosition, 1.0));
}

