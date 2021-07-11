#pragma once

#include "app/resources/CResourceManager.hpp"
#include "app/scene/CFrameBufferObject.hpp"
#include "app/scene/CVertexArrayObject.hpp"
#include "app/scene/ICamera.hpp"
#include "app/scene/IModel.hpp"
#include "app/textures/CTexture2D.hpp"
#include "ecs/EntityManager.hpp"

#include <memory>


class C3DRenderSystem
{
public:
    C3DRenderSystem(ecs::EntityManager& entityManager, const CResourceManager& resourceManager);
    void render(const glm::mat4& view, const glm::mat4& projection);
    void prepare(const ICamera* camera);

private:
    void renderEnvironment(const glm::mat4& view, const glm::mat4& projection);
    void renderForeground(const glm::mat4& view, const glm::mat4& projection);
    void renderForegroundPBR(const glm::mat4& view, const glm::mat4& projection);
    void renderInstanced(const glm::mat4& view, const glm::mat4& projection);
    void renderBoundingBoxes(const glm::mat4& view, const glm::mat4& projection);

    ecs::EntityManager& mEntityManager;
    const CResourceManager& mResourceManager;

    unsigned int mFrame;

    CVertexArrayObject mScreenQuad;
    CFrameBufferObject mFbo;

    TModelPtr mBoundingBoxModel;
};
