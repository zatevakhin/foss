
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
#include "app/resources/CRegistry.hpp"
#include "app/resources/CShaderManager.hpp"
#include "app/scene/CBoundingBox.hpp"
#include "app/scene/CFreeCamera.hpp"
#include "app/shading/CModelProgramAdapter.hpp"
#include "app/shading/CUniform.hpp"
#include "app/shading/CVertexAttribute.cpp"

#include <chrono>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


C3DRenderSystem::C3DRenderSystem(ecs::EntityManager& entityManager,
                                 const CResourceManager& resourceManager)
    : mEntityManager(entityManager)
    , mResourceManager(resourceManager)
    , mScreenQuad()
    , mFrame(0)
    , mBoundingBoxModel(nullptr)
    , mFbo()
{
    CBoundingBox box;
    mBoundingBoxModel = box.getModel();

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

    auto textures = mResourceManager.getTextureManager();

    auto color = textures->create<CTexture2D>("fbo.rgb", ETextureType::VIRTUAL);
    auto depth = textures->create<CTexture2D>("fbo.d.s", ETextureType::VIRTUAL);

    gl::TTexParametriList params;
    // Filtering
    params.emplace_back(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    params.emplace_back(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::ivec2 size(1920, 1080);
    color->bind();
    gl::tex_parameteri(params);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    color->unbind();

    depth->bind();
    gl::tex_parameteri(params);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_STENCIL,
                 GL_UNSIGNED_INT_24_8, 0);
    depth->unbind();

    mFbo.bind();
    mFbo.attachTexture(color->id(), GL_COLOR_ATTACHMENT0);
    mFbo.attachTexture(depth->id(), GL_DEPTH_STENCIL_ATTACHMENT);
    mFbo.unbind();
}

void C3DRenderSystem::prepare(const ICamera* camera)
{
}

void C3DRenderSystem::render(const glm::mat4& view, const glm::mat4& projection)
{
    auto shaders = mResourceManager.get_shader_manager();
    auto textures = mResourceManager.getTextureManager();

    auto colorTexture = textures->get<CTexture2D>("fbo.rgb");

    mFrame++;

    gl::enable(GL_DEPTH_TEST);

    gl::clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mFbo.bind();
    gl::clear_color(0.f, 0.f, 0.f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderForeground(view, projection);
    renderBoundingBoxes(view, projection);

    renderEnvironment(view, projection);
    renderInstanced(view, projection);
    mFbo.unbind();

    auto screen = shaders->getByName("screen");

    screen->use();
    screen->uniform("frameNumber") = static_cast<int>(mFrame);

    mScreenQuad.bind();
    colorTexture->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    colorTexture->unbind();
    mScreenQuad.unbind();

    glUseProgram(0U); // Free shader
}

void C3DRenderSystem::renderEnvironment(const glm::mat4& view, const glm::mat4& projection)
{
    auto shaders = mResourceManager.get_shader_manager();
    auto textures = mResourceManager.getTextureManager();

    auto cubeMap = textures->get<ITexture>("purple-nebula.4096");

    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);

    auto prog = shaders->getByName("skybox");

    prog->use();

    auto adapter = std::make_shared<CModelProgramAdapter>(prog);
    adapter->setProjection(projection);

    for (auto [entity, components] :
         mEntityManager.getEntitySet<CModelComponent, CSkyboxComponent>())
    {
        auto [model, skybox] = components;

        adapter->setView(glm::mat4(glm::mat3(view)));

        glActiveTexture(GL_TEXTURE0);
        cubeMap->bind();
        prog->uniform("cubemap") = 0;

        model.mModel->draw(adapter);
    }

    glFrontFace(GL_CCW);
}

void C3DRenderSystem::renderForeground(const glm::mat4& view, const glm::mat4& projection)
{
    auto shaders = mResourceManager.get_shader_manager();

    auto camera = CRegistry::get<CFreeCamera*>("camera");
    auto prog = shaders->getByName("mesh");

    std::vector<ecs::Entity> debugDraw;
    std::vector<ecs::Entity> debugDrawNormals;

    auto adapter = std::make_shared<CModelProgramAdapter>(prog);

    prog->use();
    adapter->setProjection(projection);

    prog->uniform("viewPosition") = camera->get_position();
    prog->uniform("lightPosition") = glm::vec3(13.f, 0.f, -20.f);

    for (auto [entity, components] :
         mEntityManager.getEntitySet<CModelComponent, CTransform3DComponent>())
    {
        auto [model, transform] = components;
        if (model.mIsInView && !model.mDebug.mHideModel)
        {
            adapter->setView(view * transform.toMat4());

            model.mModel->draw(adapter);
        }

        if (model.mDebug.mEnableDebugDraw)
        {
            debugDraw.push_back(entity);
        }

        if (model.mDebug.mEnableNormalsDraw)
        {
            debugDrawNormals.push_back(entity);
        }
    }

    if (debugDraw.size())
    {
        auto debug = shaders->getByName("m3d");
        debug->use();
        debug->uniform("background") = glm::vec4(1.f, 1.f, 0.f, 1.f);

        auto adapter = std::make_shared<CModelProgramAdapter>(debug);
        adapter->setProjection(projection);

        for (auto& entity : debugDraw)
        {
            auto [model, transform] =
                mEntityManager.getComponents<CModelComponent, CTransform3DComponent>(entity);

            if (model.mIsInView)
            {
                switch (model.mDebug.mDebugDrawMode)
                {
                case SModelDebug::EDebugDrawMode::FILL:
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    break;
                case SModelDebug::EDebugDrawMode::LINE:
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    break;
                case SModelDebug::EDebugDrawMode::POINT:
                    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                    break;
                default:
                    assert(true);
                    break;
                }

                adapter->setView(view * transform.toMat4());

                model.mModel->draw(adapter);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    }

    if (debugDrawNormals.size())
    {
        auto debug_normals = shaders->getByName("normals");
        debug_normals->use();

        auto adapter = std::make_shared<CModelProgramAdapter>(debug_normals);
        adapter->setProjection(projection);


        for (auto& entity : debugDrawNormals)
        {
            auto [model, transform] =
                mEntityManager.getComponents<CModelComponent, CTransform3DComponent>(entity);

            if (model.mDebug.mEnableNormalsDraw)
            {
                adapter->setView(view * transform.toMat4());
                model.mModel->draw(adapter);
            }
        }
    }
}

void C3DRenderSystem::renderBoundingBoxes(const glm::mat4& view, const glm::mat4& projection)
{
    auto shaders = mResourceManager.get_shader_manager();

    auto prog = shaders->getByName("m3d");
    prog->use();

    auto adapter = std::make_shared<CModelProgramAdapter>(prog);
    adapter->setProjection(projection);

    for (auto [entity, components] :
         mEntityManager.getEntitySet<CModelComponent, CPickingComponent, CTransform3DComponent>())
    {
        auto [model, picking, transform] = components;

        if (model.mIsInView && !model.mDebug.mHideBox)
        {
            auto aabb = model.mModel->getBoundingBox();
            glm::mat4 AABBTransform = glm::translate(glm::mat4(1), aabb.getCenter()) *
                                      glm::scale(glm::mat4(1), aabb.getSize());

            prog->uniform("background") =
                picking.isPicked ? glm::vec4(1.f, 0.f, 0.f, 1.f) : glm::vec4(0.f, 1.f, 0.f, 1.f);

            adapter->setView(view * (transform.toMat4() * AABBTransform));
            mBoundingBoxModel->draw(adapter);
        }
    }
}

void C3DRenderSystem::renderInstanced(const glm::mat4& view, const glm::mat4& projection)
{
    auto shaders = mResourceManager.get_shader_manager();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // More real alpha blend.
    // glDepthMask(GL_FALSE); // Don't write to depth buffer.

    auto program = shaders->getByName("particles");

    program->use();
    program->uniform("projection") = projection;

    for (auto [entity, component] :
         mEntityManager.getEntitySet<CParticleSystemComponent, CTransform3DComponent>())
    {
        auto [p, t] = component;

        program->uniform("modelView") = view * t.toMat4();
        p.mParticleSystem->draw(program);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
