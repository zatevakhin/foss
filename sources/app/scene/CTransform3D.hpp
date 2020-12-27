#ifndef FOSS_CORE_SCENE_CTRANSFORM3D_HPP
#define FOSS_CORE_SCENE_CTRANSFORM3D_HPP

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

#endif //FOSS_CORE_SCENE_CTRANSFORM3D_HPP
