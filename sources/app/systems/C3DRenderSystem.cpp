
#include "C3DRenderSystem.hpp"
#include "renderers/CStaticModel3DRenderer.hpp"
#include "renderers/CBoundingBox3DRenderer.hpp"
#include "renderers/CCubeMapRenderer.hpp"
#include "app/resources/CRegistry.hpp"
#include "app/resources/CShaderManager.hpp"
#include "app/scene/CCamera.hpp"
#include "SEngineSettings.hpp"
#include "geometry/CBoundingBox.hpp"

#include "app/components/CTransform3DComponent.hpp"
#include "app/components/CMeshObjectComponent.hpp"
#include "app/components/C3DModelComponent.hpp"
#include "app/components/C3dObjectComponent.hpp"
#include "app/components/CSkyboxComponent.hpp"
#include "app/components/CInstanced3dObjectComponent.hpp"

#include "app/auxiliary/trace.hpp"
#include "app/auxiliary/glm.hpp"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


#define GL_SWITCH_OPTION(expression, option) (((expression) ? glEnable : glDisable)(option))

C3DRenderSystem::C3DRenderSystem(ecs::EntityManager &entityManager, CShaderManager& shaderManager)
    : mEntityManager(entityManager)
    , mShaderManager(shaderManager)
    , mMeshRenderer()
    , mBBoxRenderer()
    , mCubeMapRenderer()
    , mStaticModelRenderer()
    , mFbo()
    , mFboTextureRgb()
    , mFboTextureDepth()
    , mScreenQuad()
{
    // test test test
    CRegistry::set("dbg.tex.fbo.rgb", &mFboTextureRgb);
    CRegistry::set("dbg.tex.fbo.depth", &mFboTextureDepth);


    std::vector<float> vtx({
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    });

    mScreenQuad.bind();
    CVertexBufferObject q_vbo(EBufferType::eAttributes);
    q_vbo.bind();
    q_vbo.copy(vtx);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    q_vbo.unbind();
    mScreenQuad.unbind();

    mFbo.bind();

    mFboTextureRgb.bind();
    mFboTextureRgb.setTexture(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, {1920, 1080}, 0);
    mFboTextureRgb.setFilter();
    mFboTextureRgb.unbind();

    mFboTextureDepth.bind();
    mFboTextureDepth.setTexture(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, {1920, 1080}, 0);
    mFboTextureDepth.setFilter();
    mFboTextureDepth.unbind();

    mFbo.attachTexture(mFboTextureRgb.id(), GL_COLOR_ATTACHMENT0);
    mFbo.attachTexture(mFboTextureDepth.id(), GL_DEPTH_STENCIL_ATTACHMENT);

    mFbo.isComplete();

    mFbo.unbind();

    mCubeMapRenderer.setProgram(mShaderManager.getByName("skybox"));
    mStaticModelRenderer.setProgram(mShaderManager.getByName("phong"));
    mMeshRenderer.setProgram(mShaderManager.getByName("m3d"));
    mBBoxRenderer.setProgram(mShaderManager.getByName("m3d"));
}

void C3DRenderSystem::prepare(const ICamera* camera)
{

    mCubeMapRenderer.setViewMatrix(glm::mat4(glm::mat3(camera->getView())));
    mStaticModelRenderer.setViewMatrix(camera->getView());
    mMeshRenderer.setViewMatrix(camera->getView());
    mBBoxRenderer.setViewMatrix(camera->getView());

    mCubeMapRenderer.setProjectionMatrix(camera->getProjection());
    mStaticModelRenderer.setProjectionMatrix(camera->getProjection());
    mMeshRenderer.setProjectionMatrix(camera->getProjection());
    mBBoxRenderer.setProjectionMatrix(camera->getProjection());
}

void C3DRenderSystem::render(const glm::mat4 & view, const glm::mat4 & projection)
{
    // For testing purposes.
    auto settings = CRegistry::get<SEngineSettings *>("settings");

    glPolygonMode(GL_FRONT_AND_BACK, settings->mPolygonMode.mItems[settings->mPolygonMode.mIndex]);
    GL_SWITCH_OPTION(settings->mDepthTest, GL_DEPTH_TEST);
    GL_SWITCH_OPTION(settings->mCullFace, GL_CULL_FACE);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderForeground(view, projection);
    renderBoundingBoxes(view, projection);

    renderInstanced(view, projection);
    renderEnvironment(view, projection);

    mFbo.bind();
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderForeground(view, projection);
    renderBoundingBoxes(view, projection);
    mFbo.unbind();


    glViewport(0, 0, 320, 200);
    glDisable(GL_DEPTH_TEST);

    mShaderManager.use("screen");
    mScreenQuad.bind();
    mFboTextureRgb.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    mFboTextureRgb.unbind();
    mScreenQuad.unbind();

    glViewport(0, 200, 320, 200);
    mShaderManager.use("depth");
    mScreenQuad.bind();
    mFboTextureDepth.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    mFboTextureDepth.unbind();
    mScreenQuad.unbind();


    glViewport(0, 0, 1920, 1080);
}

void C3DRenderSystem::renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection)
{
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);

    mCubeMapRenderer.use();

    for (auto [entity, components] : mEntityManager.getEntitySet<C3DModelComponent, CSkyboxComponent>())
    {
        auto [mesh, skybox] = components;

        mCubeMapRenderer.setTransformMatrix(glm::mat4(0.f));
        mCubeMapRenderer.draw(*mesh.mModel);
    }

    glFrontFace(GL_CCW);
}

void C3DRenderSystem::renderForeground(const glm::mat4 &view, const glm::mat4 &projection)
{
    mStaticModelRenderer.use();
    for (auto [entity, components] : mEntityManager.getEntitySet<C3DModelComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [model, drawable, transform] = components;

        if (drawable.isInCameraView)
        {
            mStaticModelRenderer.setTransformMatrix(transform.toMat4());
            mStaticModelRenderer.draw(*model.mModel);
        }
    }

    mMeshRenderer.use();
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
    mBBoxRenderer.use();
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

    for (auto [entity, components] : mEntityManager.getEntitySet<CMeshObjectComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [model, object, transform] = components;

        if (object.isInCameraView)
        {
            mBBoxRenderer.setTransformMatrix(transform.toMat4());
            mBBoxRenderer.setIsPicked(object.isPicked);
            mBBoxRenderer.draw(*model.mMeshObject);
        }
    }
}

void C3DRenderSystem::renderInstanced(const glm::mat4 &view, const glm::mat4 &projection)
{
    // for (auto [entity, components] : mEntityManager.getEntitySet<C3DModelComponent, CInstanced3dObjectComponent, CTransform3DComponent>())
    // {
        // auto [model, object, transform] = components;

        // trc_log("instanced entity = %ld", entity);
    // }
}
