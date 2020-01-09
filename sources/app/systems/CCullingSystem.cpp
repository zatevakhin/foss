
#include "CCullingSystem.hpp"
#include "app/components/CDrawableComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"
#include "app/components/CMeshComponent.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>

void CCullingSystem::update(double& delta)
{
    std::array<glm::vec4, 6> planes;

    for (const auto & entity : getEntities())
    {
        auto & c = entity.getComponent<CDrawableComponent>();
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

        glm::mat4 matrix = mProjection * mView * transform.toMat4();

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
        for (const auto &plane : planes)
        {
            if (glm::dot(glm::vec3(plane), center) + glm::dot(glm::abs(glm::vec3(plane)), extent) < -plane.w)
            {
                inFrustum = true;
                break;
            }
        }

        c.isInCameraView = inFrustum;
    }

}
