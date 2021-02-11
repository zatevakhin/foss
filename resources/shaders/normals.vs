#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;


out Vertex
{
    vec4 normal;
    vec4 color;
} vertex;


void main()
{
    gl_Position = vec4(position, 1.0);
    vertex.normal = vec4(normal, 1.0);
    vertex.color =  vec4(1.0, 1.0, 0.0, 1.0);
}
