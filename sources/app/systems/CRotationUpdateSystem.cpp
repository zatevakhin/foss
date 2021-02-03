
#include "CRotationUpdateSystem.hpp"
#include "app/components/CTransform3DComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


CRotationUpdateSystem::CRotationUpdateSystem(ecs::EntityManager& entityManager)
    : mEntityManager(entityManager)
{
}

void CRotationUpdateSystem::update(double& delta)
{
}
