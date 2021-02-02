#version 450 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

out vec2 fragTextureUV;

uniform mat4 projection;
uniform mat4 view;


void main() {
    fragTextureUV = tex_coord;
    gl_Position = projection * (view * vec4(vertex, 1.0));
}

