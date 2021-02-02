#pragma once

#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/renderers/CMeshRenderer.hpp"
#include "app/renderers/CParticleSystemRenderer.hpp"
#include "app/scene/CCompleteFbo.hpp"
#include "app/scene/ICamera.hpp"
#include "app/textures/CTexture2D.hpp"
#include "ecs/EntityManager.hpp"

#include <memory>


class CShaderManager;

using TShaderManagerPtr = std::shared_ptr<CShaderManager>;


class C3DRenderSystem
{
public:
    C3DRenderSystem(ecs::EntityManager& entityManager, TShaderManagerPtr shader_manager);
    void render(const glm::mat4& view, const glm::mat4& projection);
    void prepare(const ICamera* camera);

private:
    void renderEnvironment(const glm::mat4& view, const glm::mat4& projection);
    void renderForeground(const glm::mat4& view, const glm::mat4& projection);
    void renderInstanced(const glm::mat4& view, const glm::mat4& projection);
    void renderBoundingBoxes(const glm::mat4& view, const glm::mat4& projection);

    CMeshRenderer mMeshRenderer;
    CBoundingBox3DRenderer mBBoxRenderer;
    CParticleSystemRenderer mParticleSystemRenderer;

    ecs::EntityManager& mEntityManager;
    TShaderManagerPtr m_shader_manager;

    CCompleteFbo mFbo;

    CVertexArrayObject mScreenQuad;
};
