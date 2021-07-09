#pragma once

#include "app/textures/ITexture.hpp"

#include <glm/vec4.hpp>
#include <memory>


struct SMaterialPhong
{
    float mShininess{0.f};
    float mSpecular{1.f};

    glm::vec4 mDiffuseColor{0.f};
    glm::vec4 mSpecularColor{0.f};
    glm::vec4 mEmissiveColor{0.f};

    TTextureSharedPtr mDiffuseTexture;
    TTextureSharedPtr mSpecularTexture;
    TTextureSharedPtr mEmissiveTexture;
};

using TPhongMaterialPtr = std::shared_ptr<SMaterialPhong>;
