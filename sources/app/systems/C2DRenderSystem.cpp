
#include "C2DRenderSystem.hpp"
#include "core/auxiliary/trace.hpp"


void C2DRenderSystem::render(const glm::mat4 & view, const glm::mat4 & projection)
{
    for (const auto &entity : getEntities())
    {
        const auto &compponent = entity.getComponent<CWindowComponent>();
        compponent.mWindow->draw();
    }
}

