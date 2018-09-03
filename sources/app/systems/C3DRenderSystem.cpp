
#include "C3DRenderSystem.hpp"
#include "app/renderers/CStaticModel3DRenderer.hpp"
#include "app/renderers/CCubeMapRenderer.hpp"
#include "core/resources/CRegistry.hpp"
#include "SEngineSettings.hpp"

#include "core/auxiliary/trace.hpp"
#include "core/auxiliary/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>

#define GL_SWITCH_OPTION(expression, option) (((expression) ? glEnable : glDisable)(option))

void C3DRenderSystem::render(const glm::mat4 & view, const glm::mat4 & projection)
{   
    auto settings = CRegistry::get<SEngineSettings*>("settings");

    glPolygonMode(GL_FRONT_AND_BACK, settings->mPolygonMode.mItems[settings->mPolygonMode.mIndex]);
    GL_SWITCH_OPTION(settings->mDepthTest, GL_DEPTH_TEST);
    GL_SWITCH_OPTION(settings->mCullFace, GL_CULL_FACE);
    
    glFrontFace(GL_CCW);
    renderForeground(view, projection);

    glFrontFace(GL_CW);
    glDepthFunc(GL_LEQUAL);
    renderEnvironment(view, projection);

}

void C3DRenderSystem::renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection)
{
    CCubeMapRenderer renderer;

    renderer.use(mEnvironmentProgram);
    renderer.setViewMatrix(glm::mat4(glm::mat3(view)));
    renderer.setProjectionMatrix(projection);

    for (const auto &entity : getEntities())
    {
        const auto &mesh = entity.getComponent<CMeshComponent>();

        if (CMeshComponent::ECategory::eEnvironment != mesh.mCategory)
        {
            continue;
        }
        
        renderer.setTransformMatrix(glm::mat4(0.f));
        renderer.draw(*mesh.mModel);
    }
}

void C3DRenderSystem::renderForeground(const glm::mat4 & view, const glm::mat4 & projection)
{
    CStaticModel3DRenderer renderer;

    renderer.use(mForegroundPhongProgram);
    renderer.setViewMatrix(view);
    renderer.setProjectionMatrix(projection);

    for (const auto &entity : getEntities())
    {
        const auto &mesh = entity.getComponent<CMeshComponent>();

        if (CMeshComponent::ECategory::eForeground != mesh.mCategory)
        {
            continue;
        }
        
        const auto &transform = entity.getComponent<CTransform3DComponent>();
        renderer.setTransformMatrix(transform.toMat4());
        renderer.draw(*mesh.mModel);
    }
}
