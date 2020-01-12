
#ifndef FOSS_C3DRENDERSYSTEM_HPP
#define FOSS_C3DRENDERSYSTEM_HPP

#include "app/shading/C3DModelProgram.hpp"
#include "app/shading/CCubeMapProgram.hpp"
#include "app/shading/CPhongProgram.hpp"

#include "app/renderers/CCubeMapRenderer.hpp"
#include "app/renderers/CStaticModel3DRenderer.hpp"
#include "app/renderers/CBoundingBox3DRenderer.hpp"

#include "ecs/EntityManager.hpp"

class CBoundingBox3DRenderer;


class C3DRenderSystem
{
public:
    C3DRenderSystem(ecs::EntityManager &entityManager);
    void render(const glm::mat4 & view, const glm::mat4 & projection);

private:
    void renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection);
    void renderForeground(const glm::mat4 & view, const glm::mat4 & projection);
    void renderInstanced(const glm::mat4 & view, const glm::mat4 & projection);
    void renderBoundingBoxes(const glm::mat4 & view, const glm::mat4 & projection);

    CCubeMapProgram mEnvironmentProgram;
    CPhongProgram mForegroundPhongProgram;
    C3DModelProgram m3DModelProgram;

    CCubeMapRenderer mCubeMapRenderer;
    CStaticModel3DRenderer mStaticModelRenderer;
    CBoundingBox3DRenderer mBBoxRenderer;

    ecs::EntityManager &mEntityManager;
};

#endif //FOSS_C3DRENDERSYSTEM_HPP
