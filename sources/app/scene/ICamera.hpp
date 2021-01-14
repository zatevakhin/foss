#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>


class ICamera
{

public:
    virtual void update(double delta) = 0;

    virtual glm::mat4 getView() const = 0;
    virtual glm::mat4 getProjection() const = 0;
    virtual glm::vec3 getPosition() const = 0;
};
