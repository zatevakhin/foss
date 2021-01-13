#version 330 core

layout (location = 0) in vec3 particlePosition;
layout (location = 1) in vec2 textureUV;

out vec2 fragTextureUV;

uniform mat4 modelView;
uniform mat4 projection;
uniform vec3 sizeScale;

void main(void)
{
    // Particle should be always oriented to viewer,
    //  but particle system size scale still applies.
    vec4 particleCenter = modelView * vec4(particlePosition, 1.0);
    vec2 offset = textureUV * sizeScale.xy;
    vec4 vertexPos = particleCenter + vec4(offset.x, offset.y, 0.0, 0.0);

    // Normalize UV from [-1..1] to [0..1]
    fragTextureUV = 0.5 * textureUV + vec2(0.5);

    gl_Position = projection * vertexPos;
}
