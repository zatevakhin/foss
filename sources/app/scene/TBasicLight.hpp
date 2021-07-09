#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>


struct TBasicLight
{
    glm::vec3 mPosition;
    glm::vec4 mColor;

    TBasicLight(const glm::vec3& position, const glm::vec4& color)
        : mPosition(position)
        , mColor(color)
    {
    }

    ~TBasicLight() = default;
};


using TBasicLightPtr = std::shared_ptr<TBasicLight>;
