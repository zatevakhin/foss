
#include "CCullingSystem.hpp"
#include "app/components/C3dObjectComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"
#include "app/components/CMeshComponent.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>

CCullingSystem::CCullingSystem(ecs::EntityManager &entityManager)
    : mEntityManager(entityManager)
{
}

void CCullingSystem::update(double& delta)
{
    std::array<glm::vec4, 6> planes;

    for (auto [entity, components] : mEntityManager.getEntitySet<CMeshComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [mesh, object, transform] = components;

        const auto & aabb = mesh.mModel->mGeometry->getBoundingBox();
        const auto & bounds = aabb.getBounds<glm::vec3>();

        glm::vec3 center = glm::vec3((bounds.mMin.x + bounds.mMax.x) / 2, (bounds.mMin.y + bounds.mMax.y) / 2, (bounds.mMin.z + bounds.mMax.z) / 2);
        glm::vec3 extent = bounds.mMax;

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


        object.isInCameraView =  ([&]{
            bool result = true;

            for (const auto &plane : planes)
            {
                if (glm::dot(glm::vec3(plane), center) + glm::dot(glm::abs(glm::vec3(plane)), extent) < -plane.w)
                {
                    result = false;
                    break;
                }
            }

            return result;
        })();
    }

}
