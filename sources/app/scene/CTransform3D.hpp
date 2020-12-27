#pragma once

#include "app/auxiliary/glm.hpp"


class CTransform3D
{
public:
    CTransform3D();

    glm::mat4 toMat4() const;

    glm::vec3 mScale;
    glm::quat mOrientation;
    glm::vec3 mPosition;
};
