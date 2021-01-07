#pragma once

#include "app/renderers/CMeshRenderer.hpp"
#include "app/renderers/CCubeMapRenderer.hpp"
#include "app/renderers/CStaticModel3DRenderer.hpp"
#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/scene/ICamera.hpp"
#include "app/scene/CFrameBufferObject.hpp"
#include "app/textures/CTexture2D.hpp"

#include "ecs/EntityManager.hpp"

class CShaderManager;

class C3DRenderSystem
{
public:
    C3DRenderSystem(ecs::EntityManager& entityManager, CShaderManager& shaderManager);
    void render(const glm::mat4 & view, const glm::mat4 & projection);
    void prepare(const ICamera* camera);

private:
    void renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection);
    void renderForeground(const glm::mat4 & view, const glm::mat4 & projection);
    void renderInstanced(const glm::mat4 & view, const glm::mat4 & projection);
    void renderBoundingBoxes(const glm::mat4 & view, const glm::mat4 & projection);

    CMeshRenderer mMeshRenderer;
    CCubeMapRenderer mCubeMapRenderer;
    CStaticModel3DRenderer mStaticModelRenderer;
    CBoundingBox3DRenderer mBBoxRenderer;

    ecs::EntityManager &mEntityManager;
    CShaderManager &mShaderManager;

    CFrameBufferObject mFbo;
    CTexture2D mFboTextureRgb;
    CTexture2D mFboTextureDepth;
    GLuint mRbo;

    CVertexArrayObject mScreenQuad;
};
