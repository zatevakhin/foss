
#include "CRotationUpdateSystem.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


CRotationUpdateSystem::CRotationUpdateSystem()
{
}

void CRotationUpdateSystem::update(double& delta)
{
    const auto x = static_cast<float>(delta / 360);

    for (const auto & entity : getEntities())
    {
        auto & c = entity.getComponent<CTransform3DComponent>();

        c.mOrientation = glm::rotate(c.mOrientation, x, glm::vec3(1, 0, 0));
        c.mOrientation = glm::rotate(c.mOrientation, x, glm::vec3(0, 1, 0));
        c.mOrientation = glm::rotate(c.mOrientation, x, glm::vec3(0, 0, 1));
    }
}