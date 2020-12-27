
#include "CTransform3D.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "app/auxiliary/trace.hpp"

CTransform3D::CTransform3D()
    : mScale(glm::vec3(1))
    , mOrientation(glm::angleAxis(0.f, glm::vec3(0, 1, 0)))
    , mPosition(glm::vec3(0))
{
}

glm::mat4 CTransform3D::toMat4() const
{
    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), mScale);
    const glm::mat4 rotationMatrix = glm::mat4_cast(mOrientation);
    const glm::mat4 translateMatrix = glm::translate(glm::mat4(1), mPosition);
    return translateMatrix * rotationMatrix * scaleMatrix;
}