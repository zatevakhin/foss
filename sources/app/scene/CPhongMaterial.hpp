#pragma once

#include "IMaterial.hpp"
#include "app/textures/ITexture.hpp"

#include <glm/vec4.hpp>
#include <memory>


class CPhongMaterial : public IMaterial
{
public:
    float mShininess{0.f};
    float mSpecular{1.f};

    glm::vec4 mDiffuseColor{0.f};
    glm::vec4 mSpecularColor{0.f};
    glm::vec4 mEmissiveColor{0.f};

    TTextureSharedPtr mDiffuseTexture;
    TTextureSharedPtr mSpecularTexture;
    TTextureSharedPtr mEmissiveTexture;

    CPhongMaterial() = default;

    virtual ~CPhongMaterial() = default;

    virtual bool isPbr() const override
    {
        return false;
    }
};

using TPhongMaterialPtr = std::shared_ptr<CPhongMaterial>;
