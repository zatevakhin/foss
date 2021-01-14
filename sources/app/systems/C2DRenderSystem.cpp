
#include "C2DRenderSystem.hpp"
#include "app/auxiliary/trace.hpp"

#include "components/CWindowComponent.hpp"


C2DRenderSystem::C2DRenderSystem(ecs::EntityManager& entityManager)
    : mEntityManager(entityManager)
{
}


void C2DRenderSystem::render(const glm::mat4& view, const glm::mat4& projection)
{
    for (auto [entity, components] : mEntityManager.getEntitySet<CWindowComponent>())
    {
        auto [window] = components;

        window.mWindow->draw();
    }
}
