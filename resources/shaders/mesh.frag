#version 450 core

out vec4 FragColor;

in vec2 fragTextureUV;
in vec3 Normal;
in vec3 FragmentPosition;


struct Material
{
    float shininess;

    vec4 colorDiffuse;
    vec4 colorSpecular;
    vec4 colorEmissive;

    sampler2D textureDiffuse;
    sampler2D textureSpecular;
    sampler2D textureEmissive;
};

uniform Material material;
uniform vec3 lightPosition;
uniform vec3 viewPosition;


void main()
{
    vec3 color = texture(material.textureDiffuse, fragTextureUV.st).rgb;

    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3 lightDir = normalize(lightPosition - FragmentPosition);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 viewDir = normalize(viewPosition - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
