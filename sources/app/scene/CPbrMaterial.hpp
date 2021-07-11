#pragma once

#include "app/scene/IMaterial.hpp"

#include <glm/vec3.hpp>
#include <memory>


class CPbrMaterial : public IMaterial
{

public:
    glm::vec3 albedo{0.f};

    float ao{0.f};
    float metallic{0.f};
    float roughness{0.f}; // 0.05-1.0

    // textures...

    CPbrMaterial() = default;
    virtual ~CPbrMaterial() override = default;

    virtual bool isPbr() const override
    {
        return true;
    }
};

using TPbrMaterialPtr = std::shared_ptr<CPbrMaterial>;
