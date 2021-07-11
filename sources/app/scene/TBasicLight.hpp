#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>


struct TBasicLight
{
    glm::vec3 mPosition;
    glm::vec3 mColor;
    float mStrength;

    TBasicLight(const glm::vec3& position, const glm::vec3& color)
        : mPosition(position)
        , mColor(color)
        , mStrength(1.f)
    {
    }

    ~TBasicLight() = default;
};


using TBasicLightPtr = std::shared_ptr<TBasicLight>;
