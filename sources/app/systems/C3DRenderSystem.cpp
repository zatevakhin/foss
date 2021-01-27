
#include "C3DRenderSystem.hpp"
#include "SEngineSettings.hpp"

#include "app/auxiliary/glm.hpp"
#include "app/auxiliary/trace.hpp"
#include "app/components/C3DModelComponent.hpp"
#include "app/components/C3dObjectComponent.hpp"
#include "app/components/CInstanced3dObjectComponent.hpp"
#include "app/components/CMeshObjectComponent.hpp"
#include "app/components/CParticleSystemComponent.hpp"
#include "app/components/CSkyboxComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"
#include "app/components/MeshComponent.hpp"
#include "app/geometry/CBoundingBox.hpp"
#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/renderers/CCubeMapRenderer.hpp"
#include "app/renderers/CStaticModel3DRenderer.hpp"
#include "app/resources/CRegistry.hpp"
#include "app/resources/CShaderManager.hpp"
#include "app/scene/CCamera.hpp"
#include "app/shading/CUniform.hpp"
#include "app/shading/CVertexAttribute.cpp"


#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


#define GL_SWITCH_OPTION(expression, option) (((expression) ? gl::enable : gl::disable)(option))


C3DRenderSystem::C3DRenderSystem(ecs::EntityManager& entityManager,
                                 TShaderManagerPtr shader_manager)
    : mEntityManager(entityManager)
    , m_shader_manager(shader_manager)
    , mMeshRenderer()
    , mBBoxRenderer()
    , mCubeMapRenderer()
    , mStaticModelRenderer()
    , mParticleSystemRenderer()
    , mFbo({1920, 1080})
    , mScreenQuad()
    , mParticlesVao()
{

    std::vector<float> vtx({-1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                            1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
                            1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f});

    std::vector<glm::vec3> vtxParticles;

    unsigned int amount = 1000;
    float radius = 50.0;
    float offset = 10.f;

    for (size_t i = 0; i < amount; ++i)
    {
        float angle = i / amount * 360.0f;

        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;


        vtxParticles.push_back(glm::vec3(x, y, z));
    }


    mParticlesVao.bind();
    CVertexBufferObject particlesVbo(EBufferType::eArrayBuffer);
    particlesVbo.bind();
    particlesVbo.copy(vtxParticles);
    gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    particlesVbo.unbind();
    mParticlesVao.unbind();


    mScreenQuad.bind();
    CVertexBufferObject q_vbo(EBufferType::eArrayBuffer);
    q_vbo.bind();
    q_vbo.copy(vtx);
    gl::vertex_attrib_pointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    gl::vertex_attrib_pointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                              (GLvoid*)(2 * sizeof(GLfloat)));
    q_vbo.unbind();
    mScreenQuad.unbind();

    mCubeMapRenderer.setProgram(m_shader_manager->getByName("skybox"));
    mStaticModelRenderer.setProgram(m_shader_manager->getByName("phong"));
    mMeshRenderer.setProgram(m_shader_manager->getByName("m3d"));
    mBBoxRenderer.setProgram(m_shader_manager->getByName("m3d"));
}

void C3DRenderSystem::prepare(const ICamera* camera)
{
    // for (auto [entity, component] :
    //      mEntityManager.getEntitySet<CParticleSystemComponent, CTransform3DComponent>())
    // {
    //     auto [c, t] = component;
    //     t.mPosition = camera->getPosition();
    // }

    auto view = camera->get_view();
    auto projection = camera->get_projection();

    mCubeMapRenderer.setViewMatrix(glm::mat4(glm::mat3(view)));
    mStaticModelRenderer.setViewMatrix(view);
    mMeshRenderer.setViewMatrix(view);
    mBBoxRenderer.setViewMatrix(view);

    mCubeMapRenderer.setProjectionMatrix(projection);
    mStaticModelRenderer.setProjectionMatrix(projection);
    mMeshRenderer.setProjectionMatrix(projection);
    mBBoxRenderer.setProjectionMatrix(projection);
}

void C3DRenderSystem::render(const glm::mat4& view, const glm::mat4& projection)
{
    // For testing purposes.
    auto settings = CRegistry::get<SEngineSettings*>("settings");

    glPolygonMode(GL_FRONT_AND_BACK, settings->mPolygonMode.mItems[settings->mPolygonMode.mIndex]);
    GL_SWITCH_OPTION(settings->mDepthTest, GL_DEPTH_TEST);
    GL_SWITCH_OPTION(settings->mCullFace, GL_CULL_FACE);

    gl::clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    renderForeground(view, projection);
    renderBoundingBoxes(view, projection);

    renderEnvironment(view, projection);
    renderInstanced(view, projection);

    mFbo.bind();
    gl::clear_color(0.f, 0.f, 0.f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderForeground(view, projection);
    renderBoundingBoxes(view, projection);
    renderInstanced(view, projection);
    mFbo.unbind();


    gl::viewport(0, 0, 320, 200);
    gl::disable(GL_DEPTH_TEST);

    auto& colorTexture = mFbo.getColorTexture();
    auto& depthTexture = mFbo.getDepthTexture();

    m_shader_manager->use("screen");
    mScreenQuad.bind();
    colorTexture.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    colorTexture.unbind();
    mScreenQuad.unbind();

    gl::viewport(0, 200, 320, 200);
    m_shader_manager->use("depth");
    mScreenQuad.bind();
    depthTexture.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    depthTexture.unbind();
    mScreenQuad.unbind();


    gl::viewport(0, 0, 1920, 1080);
    glUseProgram(0U); // Free shader
}

void C3DRenderSystem::renderEnvironment(const glm::mat4& view, const glm::mat4& projection)
{
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);

    mCubeMapRenderer.use();

    for (auto [entity, components] :
         mEntityManager.getEntitySet<C3DModelComponent, CSkyboxComponent>())
    {
        auto [mesh, skybox] = components;

        mCubeMapRenderer.setTransformMatrix(glm::mat4(0.f));
        mCubeMapRenderer.draw(*mesh.mModel);
    }

    glFrontFace(GL_CCW);
}

void C3DRenderSystem::renderForeground(const glm::mat4& view, const glm::mat4& projection)
{
    mStaticModelRenderer.use();
    for (auto [entity, components] :
         mEntityManager
             .getEntitySet<C3DModelComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [model, drawable, transform] = components;

        if (drawable.isInCameraView)
        {
            mStaticModelRenderer.setTransformMatrix(transform.toMat4());
            mStaticModelRenderer.draw(*model.mModel);
        }
    }

    mMeshRenderer.use();
    for (auto [entity, components] :
         mEntityManager
             .getEntitySet<CMeshObjectComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [mesh, drawable, transform] = components;

        if (drawable.isInCameraView)
        {
            mMeshRenderer.setTransformMatrix(transform.toMat4());
            mMeshRenderer.draw(*mesh.mMeshObject);
        }
    }

    auto prog = m_shader_manager->getByName("mesh").lock();

    prog->use();
    prog->uniform("projection") = projection;

    for (auto [entity, components] :
         mEntityManager.getEntitySet<MeshComponent, CTransform3DComponent>())
    {
        auto [mesh, transform] = components;

        prog->uniform("view") = view * transform.toMat4();
        mesh.mMesh->draw(prog);
    }
}

void C3DRenderSystem::renderBoundingBoxes(const glm::mat4& view, const glm::mat4& projection)
{
    mBBoxRenderer.use();
    for (auto [entity, components] :
         mEntityManager
             .getEntitySet<C3DModelComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [model, object, transform] = components;

        if (object.isInCameraView)
        {
            mBBoxRenderer.setTransformMatrix(transform.toMat4());
            mBBoxRenderer.setIsPicked(object.isPicked);
            mBBoxRenderer.draw(*model.mModel);
        }
    }

    for (auto [entity, components] :
         mEntityManager
             .getEntitySet<CMeshObjectComponent, C3dObjectComponent, CTransform3DComponent>())
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

void C3DRenderSystem::renderInstanced(const glm::mat4& view, const glm::mat4& projection)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // More real alpha blend.
    // glDepthMask(GL_FALSE); // Don't write to depth buffer.

    auto program = m_shader_manager->getByName("particles").lock();

    program->use();
    program->uniform("projection") = projection;

    for (auto [entity, component] :
         mEntityManager.getEntitySet<CParticleSystemComponent, CTransform3DComponent>())
    {
        auto [p, t] = component;

        const auto& modelView = view * t.toMat4();
        program->uniform("modelView") = modelView;
        p.mParticleSystem->draw(program, modelView);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
