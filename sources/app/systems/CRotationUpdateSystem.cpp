
#include "CRotationUpdateSystem.hpp"
#include "app/components/C3dObjectComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


CRotationUpdateSystem::CRotationUpdateSystem(ecs::EntityManager& entityManager)
    : mEntityManager(entityManager)
{
}

void CRotationUpdateSystem::update(double& delta)
{
    // const auto x = static_cast<float>(delta / 360);

    // for (auto [entity, components] : mEntityManager.getEntitySet<C3dObjectComponent,
    // CTransform3DComponent>())
    // {
    //     auto [object, transform] = components;

    //     transform.mOrientation = glm::rotate(transform.mOrientation, x, glm::vec3(1, 0, 0));
    //     transform.mOrientation = glm::rotate(transform.mOrientation, x, glm::vec3(0, 1, 0));
    //     transform.mOrientation = glm::rotate(transform.mOrientation, x, glm::vec3(0, 0, 1));
    // }
}
