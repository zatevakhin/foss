#pragma once

#include "app/textures/ITexture.hpp"
#include <glm/mat4x4.hpp>


struct SPhongMaterial
{
    glm::vec4 mDiffuseColor;
    glm::vec4 mSpecularColor;
    glm::vec4 mEmissiveColor;

    float mShininess = 0;

    CTextureSharedPtr mDiffuse;
    CTextureSharedPtr mSpecular;
    CTextureSharedPtr mEmissive;
};
