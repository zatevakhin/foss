
#include "C3DRenderSystem.hpp"
#include "app/renderers/CStaticModel3DRenderer.hpp"
#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/renderers/CCubeMapRenderer.hpp"
#include "core/resources/CRegistry.hpp"
#include "core/scene/CCamera.hpp"
#include "SEngineSettings.hpp"
#include "core/geometry/CBoundingBox.hpp"

#include "core/auxiliary/trace.hpp"
#include "core/auxiliary/glm.hpp"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


#define GL_SWITCH_OPTION(expression, option) (((expression) ? glEnable : glDisable)(option))

C3DRenderSystem::C3DRenderSystem()
    : mBBoxRenderer()
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

    glFrontFace(GL_CW);
    glDepthFunc(GL_LEQUAL);
    renderEnvironment(view, projection);
}

void C3DRenderSystem::renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection)
{

    mCubeMapRenderer.use(mEnvironmentProgram);
    mCubeMapRenderer.setViewMatrix(glm::mat4(glm::mat3(view)));
    mCubeMapRenderer.setProjectionMatrix(projection);

    for (const auto & entity : getEntities())
    {
        const auto & mesh = entity.getComponent<CMeshComponent>();

        if (CMeshComponent::ECategory::eEnvironment != mesh.mCategory)
        {
            continue;
        }

        mCubeMapRenderer.setTransformMatrix(glm::mat4(0.f));
        mCubeMapRenderer.draw(*mesh.mModel);
    }
}

void C3DRenderSystem::renderForeground(const glm::mat4 & view, const glm::mat4 & projection)
{
    mStaticModelRenderer.use(mForegroundPhongProgram);
    mStaticModelRenderer.setViewMatrix(view);
    mStaticModelRenderer.setProjectionMatrix(projection);

    std::array<glm::vec4, 6> planes;


    for (const auto &entity : getEntities())
    {
        const auto &mesh = entity.getComponent<CMeshComponent>();

        if (CMeshComponent::ECategory::eForeground != mesh.mCategory)
        {
            continue;
        }

        const auto & transform = entity.getComponent<CTransform3DComponent>();
        const auto & aabb = mesh.mModel->mGeometry->getBoundingBox();
        const auto & bounds = aabb.getBounds<glm::vec3>();

        glm::vec3 center = glm::vec3((bounds.mMin.x + bounds.mMax.x) / 2, (bounds.mMin.y + bounds.mMax.y) / 2, (bounds.mMin.z + bounds.mMax.z) / 2);
        glm::vec3 extent = bounds.mMin;

        glm::mat4 matrix = projection * view * transform.toMat4();

        glm::vec4 rowX = glm::row(matrix, 0);
        glm::vec4 rowY = glm::row(matrix, 1);
        glm::vec4 rowZ = glm::row(matrix, 2);
        glm::vec4 rowW = glm::row(matrix, 3);

        planes[0] = (rowW + rowX);
        planes[1] = (rowW - rowX);
        planes[2] = (rowW + rowY);
        planes[3] = (rowW - rowY);
        planes[4] = (rowW + rowZ);
        planes[5] = (rowW - rowZ);

        bool inFrustum = false;
        for (const auto & plane : planes)
        {
            if (glm::dot(glm::vec3(plane), center) + glm::dot(glm::abs(glm::vec3(plane)), extent) < -plane.w)
            {
                inFrustum = true;
                break;
            }
        }

        if (inFrustum)
        {
            continue;
        }

        mStaticModelRenderer.setTransformMatrix(transform.toMat4());
        mStaticModelRenderer.draw(*mesh.mModel);
    }
}

void C3DRenderSystem::renderBoundingBoxes(const glm::mat4 & view, const glm::mat4 & projection)
{
    mBBoxRenderer.use(m3DModelProgram);
    mBBoxRenderer.setViewMatrix(view);
    mBBoxRenderer.setProjectionMatrix(projection);

    std::array<glm::vec4, 6> planes;

    for (const auto &entity : getEntities())
    {
        const auto &mesh = entity.getComponent<CMeshComponent>();

        if (CMeshComponent::ECategory::eForeground != mesh.mCategory)
        {
            continue;
        }

        const auto & transform = entity.getComponent<CTransform3DComponent>();
        const auto & aabb = mesh.mModel->mGeometry->getBoundingBox();
        const auto & bounds = aabb.getBounds<glm::vec3>();

        glm::vec3 center = glm::vec3((bounds.mMin.x + bounds.mMax.x) / 2, (bounds.mMin.y + bounds.mMax.y) / 2, (bounds.mMin.z + bounds.mMax.z) / 2);
        glm::vec3 extent = bounds.mMin;

        glm::mat4 matrix = projection * view * transform.toMat4();

        glm::vec4 rowX = glm::row(matrix, 0);
        glm::vec4 rowY = glm::row(matrix, 1);
        glm::vec4 rowZ = glm::row(matrix, 2);
        glm::vec4 rowW = glm::row(matrix, 3);

        planes[0] = (rowW + rowX);
        planes[1] = (rowW - rowX);
        planes[2] = (rowW + rowY);
        planes[3] = (rowW - rowY);
        planes[4] = (rowW + rowZ);
        planes[5] = (rowW - rowZ);

        bool inFrustum = false;
        for (const auto & plane : planes)
        {
            if (glm::dot(glm::vec3(plane), center) + glm::dot(glm::abs(glm::vec3(plane)), extent) < -plane.w)
            {
                inFrustum = true;
                break;
            }
        }

        if (inFrustum)
        {
            continue;
        }

        mBBoxRenderer.setTransformMatrix(transform.toMat4());
        mBBoxRenderer.draw(*mesh.mModel);
    }
}
