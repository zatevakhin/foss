//#version 330 core
//
//out vec4 color;
//
//uniform vec4 background;
//
//void main() {
//    color = background;
//}
//

// #version 330 core

// in vec2 TexCoords;

// out vec4 color;

// uniform sampler2D textureDiffuse;
// uniform vec4 background;

// void main() {
//     color = vec4(texture(textureDiffuse, TexCoords)) + background;
// }


#version 330 core
out vec4 color;

float near = 0.1;
float far = 1000.0;
float LinearizeDepth(float depth) {
   float z = depth * 2.0 - 1.0; // Back to NDC
   return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
   float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far to get depth in range [0,1] for visualization purposes.
   color = vec4(vec3(-0.1f, -0.0f, -0.0f) + vec3(depth), 1.0f);
}
