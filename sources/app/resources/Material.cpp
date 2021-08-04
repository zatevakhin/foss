
#include "Material.hpp"


int Material::mMaterialsCount = 0;


Material::Material()
    : mMaterialId(Material::newId())
    , mName(Material::newName())
{
}


Material::~Material()
{
}


int Material::getId() const
{
    return mMaterialId;
}


std::string Material::getName() const
{
    return mName;
}


void Material::setName(const std::string name)
{
    mName = name;
}


glm::vec4 Material::getBaseColorFactor() const
{
    return mBaseColorFactor;
}


void Material::setBaseColorFactor(const glm::vec4 factor)
{
    mBaseColorFactor = factor;
}


TTextureSharedPtr Material::getBaseColorTexture() const
{
    return mBaseColorTexture;
}


void Material::setBaseColorTexture(const TTextureSharedPtr texture)
{
    mBaseColorTexture = texture;
}


float Material::getMetallicFactor() const
{
    return mMetallicFactor;
}


void Material::setMetallicFactor(const float factor)
{
    mMetallicFactor = factor;
}


TTextureSharedPtr Material::getMetallicTexture() const
{
    return mMetallicTexture;
}


void Material::setMetallicTexture(const TTextureSharedPtr texture)
{
    mMetallicTexture = texture;
}


float Material::getRoughnessFactor() const
{
    return mRoughnessFactor;
}


void Material::setRoughnessFactor(const float factor)
{
    mRoughnessFactor = factor;
}


TTextureSharedPtr Material::getRoughnessTexture() const
{
    return mRoughnessTexture;
}


void Material::setRoughnessTexture(const TTextureSharedPtr texture)
{
    mRoughnessTexture = texture;
}


TTextureSharedPtr Material::getNormalTexture() const
{
    return mNormalTexture;
}


void Material::setNormalTexture(const TTextureSharedPtr texture, const double scale)
{
    mNormalTexture = texture;
    mNormalTextureScale = scale;
}


TTextureSharedPtr Material::getOcclusionTexture() const
{
    return mOcclusionTexture;
}


void Material::setOcclusionTexture(const TTextureSharedPtr texture, const double strength)
{
    mOcclusionTexture = texture;
    mOcclusionTextureStrength = strength;
}


TTextureSharedPtr Material::getEmissiveTexture() const
{
    return mEmissiveTexture;
}


void Material::setEmissiveTexture(const TTextureSharedPtr texture)
{
    mEmissiveTexture = texture;
}


glm::vec3 Material::getEmissiveFactor() const
{
    return mEmissiveFactor;
}


void Material::setEmissiveFactor(const glm::vec3 factor)
{
    mEmissiveFactor = factor;
}


int Material::newId()
{
    return mMaterialsCount++;
}

std::string Material::newName()
{
    return std::string("Material " + std::to_string(mMaterialId));
}
