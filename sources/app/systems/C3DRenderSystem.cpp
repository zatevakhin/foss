
#include "C3DRenderSystem.hpp"
#include "app/renderers/CStaticModel3DRenderer.hpp"
#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/renderers/CCubeMapRenderer.hpp"
#include "core/resources/CRegistry.hpp"
#include "core/scene/CCamera.hpp"
#include "SEngineSettings.hpp"
#include "core/geometry/CBoundingBox.hpp"

#include "core/auxiliary/trace.hpp"
#include "core/auxiliary/glm.hpp"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


#define GL_SWITCH_OPTION(expression, option) (((expression) ? glEnable : glDisable)(option))

C3DRenderSystem::C3DRenderSystem()
    : mBBoxRenderer()
    , mCubeMapRenderer()
    , mStaticModelRenderer()
{
}

void C3DRenderSystem::render(const glm::mat4 & view, const glm::mat4 & projection)
{
    // For testing purposes.
    auto settings = CRegistry::get<SEngineSettings *>("settings");

    glPolygonMode(GL_FRONT_AND_BACK, settings->mPolygonMode.mItems[settings->mPolygonMode.mIndex]);
    GL_SWITCH_OPTION(settings->mDepthTest, GL_DEPTH_TEST);
    GL_SWITCH_OPTION(settings->mCullFace, GL_CULL_FACE);

    glFrontFace(GL_CCW);
    renderForeground(view, projection);

    renderBoundingBoxes(view, projection);

    glFrontFace(GL_CW);
    glDepthFunc(GL_LEQUAL);
    renderEnvironment(view, projection);
}

void C3DRenderSystem::renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection)
{

    mCubeMapRenderer.use(mEnvironmentProgram);
    mCubeMapRenderer.setViewMatrix(glm::mat4(glm::mat3(view)));
    mCubeMapRenderer.setProjectionMatrix(projection);

    for (const auto & entity : getEntities())
    {
        const auto & mesh = entity.getComponent<CMeshComponent>();

        if (CMeshComponent::ECategory::eEnvironment != mesh.mCategory)
        {
            continue;
        }

        mCubeMapRenderer.setTransformMatrix(glm::mat4(0.f));
        mCubeMapRenderer.draw(*mesh.mModel);
    }
}

void C3DRenderSystem::renderForeground(const glm::mat4 &view, const glm::mat4 &projection)
{
    mStaticModelRenderer.use(mForegroundPhongProgram);
    mStaticModelRenderer.setViewMatrix(view);
    mStaticModelRenderer.setProjectionMatrix(projection);

    for (const auto &entity : getEntities())
    {
        const auto &dc = entity.getComponent<CDrawableComponent>();

        if (!dc.isInCameraView)
        {
            const auto &mesh = entity.getComponent<CMeshComponent>();

            if (CMeshComponent::ECategory::eForeground != mesh.mCategory)
            {
                continue;
            }

            const auto &transform = entity.getComponent<CTransform3DComponent>();

            mStaticModelRenderer.setTransformMatrix(transform.toMat4());
            mStaticModelRenderer.draw(*mesh.mModel);
        }
    }
}

void C3DRenderSystem::renderBoundingBoxes(const glm::mat4 &view, const glm::mat4 &projection)
{
    mBBoxRenderer.use(m3DModelProgram);
    mBBoxRenderer.setViewMatrix(view);
    mBBoxRenderer.setProjectionMatrix(projection);

    for (const auto &entity : getEntities())
    {
        const auto &dc = entity.getComponent<CDrawableComponent>();

        if (!dc.isInCameraView)
        {
            const auto &mesh = entity.getComponent<CMeshComponent>();

            if (CMeshComponent::ECategory::eForeground != mesh.mCategory)
            {
                continue;
            }

            const auto &transform = entity.getComponent<CTransform3DComponent>();

            mBBoxRenderer.setTransformMatrix(transform.toMat4());
            mBBoxRenderer.draw(*mesh.mModel);
        }
    }
}
