#pragma once

#include "app/shading/C3DModelProgram.hpp"
#include "app/shading/CCubeMapProgram.hpp"
#include "app/shading/CPhongProgram.hpp"
#include "app/shading/CScreenProgram.hpp"
#include "app/shading/CDepthProgram.hpp"

#include "app/renderers/CMeshRenderer.hpp"
#include "app/renderers/CCubeMapRenderer.hpp"
#include "app/renderers/CStaticModel3DRenderer.hpp"
#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/scene/ICamera.hpp"
#include "app/scene/CFrameBufferObject.hpp"
#include "app/textures/CTexture2D.hpp"

#include "ecs/EntityManager.hpp"


class C3DRenderSystem
{
public:
    C3DRenderSystem(ecs::EntityManager &entityManager);
    void render(const glm::mat4 & view, const glm::mat4 & projection);
    void prepare(const ICamera* camera);

private:
    void renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection);
    void renderForeground(const glm::mat4 & view, const glm::mat4 & projection);
    void renderInstanced(const glm::mat4 & view, const glm::mat4 & projection);
    void renderBoundingBoxes(const glm::mat4 & view, const glm::mat4 & projection);

    CCubeMapProgram mEnvironmentProgram;
    CPhongProgram mForegroundPhongProgram;
    C3DModelProgram m3DModelProgram;
    C3DModelProgram mMeshProgram;
    CScreenProgram mScreenProgram;
    CDepthProgram mDepthProgram;

    CMeshRenderer mMeshRenderer;
    CCubeMapRenderer mCubeMapRenderer;
    CStaticModel3DRenderer mStaticModelRenderer;
    CBoundingBox3DRenderer mBBoxRenderer;

    ecs::EntityManager &mEntityManager;

    CFrameBufferObject mFbo;
    CTexture2D mFboTextureRgb;
    CTexture2D mFboTextureDepth;
    GLuint mRbo;

    CVertexArrayObject mScreenQuad;

};
