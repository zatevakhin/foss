#version 450 core

out vec4 FragColor;

in vec3 FragmentPosition;
in vec3 Normal;
in vec2 FragTextureUV;


struct Material
{
    float shininess;
    float specular;

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
    vec3 color = texture(material.textureDiffuse, FragTextureUV.st).rgb;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragmentPosition);
    vec3 viewDir = normalize(viewPosition - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // ambient
    vec3 ambient = 0.05 * color + vec3(material.colorEmissive) ;

    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color + vec3(material.colorDiffuse);

    // specular
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess); // blinn-phong
    // float spec = pow(max(dot(normal, reflectDir), 0.0), material.shininess); // phong

    vec3 specular = vec3(material.specular) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
