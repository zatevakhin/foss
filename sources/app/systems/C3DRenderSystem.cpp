
#include "C3DRenderSystem.hpp"
#include "renderers/CStaticModel3DRenderer.hpp"
#include "renderers/CBoundingBox3DRenderer.hpp"
#include "renderers/CCubeMapRenderer.hpp"
#include "base/resources/CRegistry.hpp"
#include "base/scene/CCamera.hpp"
#include "SEngineSettings.hpp"
#include "base/geometry/CBoundingBox.hpp"

#include "app/components/CTransform3DComponent.hpp"
#include "app/components/CMeshObjectComponent.hpp"
#include "app/components/C3DModelComponent.hpp"
#include "app/components/C3dObjectComponent.hpp"
#include "app/components/CSkyboxComponent.hpp"
#include "app/components/CInstanced3dObjectComponent.hpp"

#include "base/auxiliary/trace.hpp"
#include "base/auxiliary/glm.hpp"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


#define GL_SWITCH_OPTION(expression, option) (((expression) ? glEnable : glDisable)(option))

C3DRenderSystem::C3DRenderSystem(ecs::EntityManager &entityManager)
    : mEntityManager(entityManager)
    , mMeshRenderer()
    , mBBoxRenderer()
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

    // renderMinMax(view, projection);
    
    renderInstanced(view, projection);

    glFrontFace(GL_CW);
    glDepthFunc(GL_LEQUAL);
    
    renderEnvironment(view, projection);
}

void C3DRenderSystem::renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection)
{

    mCubeMapRenderer.use(mEnvironmentProgram);
    mCubeMapRenderer.setViewMatrix(glm::mat4(glm::mat3(view)));
    mCubeMapRenderer.setProjectionMatrix(projection);

    for (auto [entity, components] : mEntityManager.getEntitySet<C3DModelComponent, CSkyboxComponent>())
    {
        auto [mesh, skybox] = components;

        mCubeMapRenderer.setTransformMatrix(glm::mat4(0.f));
        mCubeMapRenderer.draw(*mesh.mModel);
    }
}

void C3DRenderSystem::renderForeground(const glm::mat4 &view, const glm::mat4 &projection)
{
    mStaticModelRenderer.use(mForegroundPhongProgram);
    mStaticModelRenderer.setViewMatrix(view);
    mStaticModelRenderer.setProjectionMatrix(projection);

    for (auto [entity, components] : mEntityManager.getEntitySet<C3DModelComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [model, drawable, transform] = components;

        if (drawable.isInCameraView)
        {
            mStaticModelRenderer.setTransformMatrix(transform.toMat4());
            mStaticModelRenderer.draw(*model.mModel);
        }
    }

    mMeshRenderer.use(m3DModelProgram);
    mMeshRenderer.setViewMatrix(view);
    mMeshRenderer.setProjectionMatrix(projection);
    for (auto [entity, components] : mEntityManager.getEntitySet<CMeshObjectComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [mesh, drawable, transform] = components;

        if (drawable.isInCameraView)
        {
            mMeshRenderer.setTransformMatrix(transform.toMat4());
            mMeshRenderer.draw(*mesh.mMeshObject);
        }
    }
}

void C3DRenderSystem::renderBoundingBoxes(const glm::mat4 &view, const glm::mat4 &projection)
{
    mBBoxRenderer.use(m3DModelProgram);
    mBBoxRenderer.setViewMatrix(view);
    mBBoxRenderer.setProjectionMatrix(projection);

    for (auto [entity, components] : mEntityManager.getEntitySet<C3DModelComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [model, object, transform] = components;

        if (object.isInCameraView)
        {
            mBBoxRenderer.setTransformMatrix(transform.toMat4());
            mBBoxRenderer.setIsPicked(object.isPicked);
            mBBoxRenderer.draw(*model.mModel);
        }
    }
}

void C3DRenderSystem::renderInstanced(const glm::mat4 &view, const glm::mat4 &projection)
{
    for (auto [entity, components] : mEntityManager.getEntitySet<C3DModelComponent, CInstanced3dObjectComponent, CTransform3DComponent>())
    {
        auto [model, object, transform] = components;

        // trc_log("instanced entity = %ld", entity);`
    }
}
