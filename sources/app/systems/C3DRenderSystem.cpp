
#include "C3DRenderSystem.hpp"
#include "SEngineSettings.hpp"

#include "app/auxiliary/glm.hpp"
#include "app/auxiliary/trace.hpp"

#include "app/components/CModelComponent.hpp"
#include "app/components/CParticleSystemComponent.hpp"
#include "app/components/CPickingComponent.hpp"
#include "app/components/CSkyboxComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"

#include "app/geometry/CBoundingBox.hpp"
#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/resources/CRegistry.hpp"
#include "app/resources/CShaderManager.hpp"
#include "app/scene/CFreeCamera.hpp"
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
    , mBBoxRenderer()
    , mFbo({1920, 1080})
    , mScreenQuad()
{

    std::vector<float> vtx({-1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                            1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
                            1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f});

    std::vector<glm::vec3> vtxParticles;


    mScreenQuad.bind();
    CVertexBufferObject q_vbo(EBufferType::eArrayBuffer);
    q_vbo.bind();
    q_vbo.copy(vtx);
    gl::vertex_attrib_pointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    gl::vertex_attrib_pointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                              (GLvoid*)(2 * sizeof(GLfloat)));
    q_vbo.unbind();
    mScreenQuad.unbind();

    mBBoxRenderer.setProgram(m_shader_manager->getByName("m3d"));
}

void C3DRenderSystem::prepare(const ICamera* camera)
{
    auto view = camera->get_view();
    auto projection = camera->get_projection();

    mBBoxRenderer.setViewMatrix(view);
    mBBoxRenderer.setProjectionMatrix(projection);
}

void C3DRenderSystem::render(const glm::mat4& view, const glm::mat4& projection)
{
    // For testing purposes.
    auto settings = CRegistry::get<SEngineSettings*>("settings");
    gl::enable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, settings->mPolygonMode.mItems[settings->mPolygonMode.mIndex]);

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

    // mCubeMapRenderer.use();

    auto prog = m_shader_manager->getByName("skybox").lock();
    auto cubeMap = CRegistry::get<TTextureSharedPtr>("texture/skybox");

    prog->use();

    for (auto [entity, components] :
         mEntityManager.getEntitySet<CModelComponent, CSkyboxComponent>())
    {
        auto [model, skybox] = components;

        prog->uniform("projection") = projection;
        prog->uniform("view") = glm::mat4(glm::mat3(view));

        glActiveTexture(GL_TEXTURE0);
        cubeMap->bind();
        prog->uniform("cubemap") = 0;

        model.mModel->draw(prog);
    }

    glFrontFace(GL_CCW);
}

void C3DRenderSystem::renderForeground(const glm::mat4& view, const glm::mat4& projection)
{
    auto camera = CRegistry::get<CFreeCamera*>("camera");
    auto prog = m_shader_manager->getByName("mesh").lock();

    prog->use();
    prog->uniform("projection") = projection;
    prog->uniform("viewPosition") = camera->get_position();
    prog->uniform("lightPosition") = glm::vec3(13.f, 0.f, -20.f);

    for (auto [entity, components] :
         mEntityManager.getEntitySet<CModelComponent, CTransform3DComponent>())
    {
        auto [model, transform] = components;
        if (model.mIsInView)
        {
            prog->uniform("view") = view * transform.toMat4();
            model.mModel->draw(prog);
        }
    }
}

void C3DRenderSystem::renderBoundingBoxes(const glm::mat4& view, const glm::mat4& projection)
{
    mBBoxRenderer.use();
    for (auto [entity, components] :
         mEntityManager.getEntitySet<CModelComponent, CPickingComponent, CTransform3DComponent>())
    {
        auto [model, picking, transform] = components;

        if (model.mIsInView)
        {
            mBBoxRenderer.setTransformMatrix(transform.toMat4());
            mBBoxRenderer.setIsPicked(picking.isPicked);
            mBBoxRenderer.draw(model.mModel);
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
