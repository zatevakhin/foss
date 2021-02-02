#version 450 core

out vec4 FragColor;
in vec2 fragTextureUV;

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



void main()
{
    float factors_diffuse = 0;
    float factors_specular = 0;

    vec4 matDiffuse = material.colorDiffuse + texture(material.textureDiffuse, fragTextureUV.st);
    vec4 matSpecular = material.colorSpecular + texture(material.textureSpecular, fragTextureUV.st);
    vec4 matEmissive = material.colorEmissive + texture(material.textureEmissive, fragTextureUV.st);

    vec4 diffuseIntensity = matDiffuse * factors_diffuse;
    vec4 specularIntensity = matSpecular * factors_specular;

    FragColor = diffuseIntensity + specularIntensity + matEmissive;
}
