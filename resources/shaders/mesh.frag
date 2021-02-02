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
    // FragColor = texture(material.textureDiffuse, fragTextureUV.st);

    vec4 matDiffuse = /* material.colorDiffuse + */ texture(material.textureDiffuse, fragTextureUV.st);
    vec4 matSpecular = material.colorSpecular + texture(material.textureSpecular, fragTextureUV.st);
    vec4 matEmissive = material.colorEmissive + texture(material.textureEmissive, fragTextureUV.st);

    FragColor = matDiffuse /* + matSpecular + matEmissive */;
}
