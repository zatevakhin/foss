
#include "CParticleUpdateSystem.hpp"
#include "app/components/CParticleSystemComponent.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>


CParticleUpdateSystem::CParticleUpdateSystem(ecs::EntityManager& entityManager)
    : mEntityManager(entityManager)
{
}

void CParticleUpdateSystem::update(double& delta)
{
    for (auto [entity, component] : mEntityManager.getEntitySet<CParticleSystemComponent>())
    {
        auto [c] = component;

        c.mParticleSystem->advance(delta);
    }
}
