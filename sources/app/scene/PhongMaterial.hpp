#pragma once

#include "app/textures/ITexture.hpp"

#include <glm/vec4.hpp>
#include <memory>


struct SMaterialPhong;

using TPhongMaterialPtr = std::shared_ptr<SMaterialPhong>;


struct SMaterialPhong
{
    float mShininess = 0;

    glm::vec4 mDiffuseColor;
    glm::vec4 mSpecularColor;
    glm::vec4 mEmissiveColor;

    TTextureSharedPtr mDiffuseTexture;
    TTextureSharedPtr mSpecularTexture;
    TTextureSharedPtr mEmissiveTexture;
};
