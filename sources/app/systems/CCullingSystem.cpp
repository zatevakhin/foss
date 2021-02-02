
#include "CCullingSystem.hpp"
#include "app/components/C3dObjectComponent.hpp"
#include "app/components/CMeshObjectComponent.hpp"
#include "app/components/CModelComponent.hpp"
#include "app/components/CTransform3DComponent.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>


CCullingSystem::CCullingSystem(ecs::EntityManager& entityManager)
    : mEntityManager(entityManager)
{
}

void CCullingSystem::update(double& delta)
{
    for (auto [entity, components] :
         mEntityManager.getEntitySet<CModelComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [model, object, transform] = components;
        cullObjects(model.mModel->getBoundingBox(), object.isInCameraView, transform.toMat4());
    }

    for (auto [entity, components] :
         mEntityManager
             .getEntitySet<CMeshObjectComponent, C3dObjectComponent, CTransform3DComponent>())
    {
        auto [mesh, object, transform] = components;
        cullObjects(mesh.mMeshObject->getBoundingBox(), object.isInCameraView, transform.toMat4());
    }
}

void CCullingSystem::cullObjects(const geometry::CBoundingBox& aabb, bool& isInCameraView,
                                 const glm::mat4 transform)
{
    const auto& bounds = aabb.getBounds<glm::vec3>();

    glm::vec3 center = aabb.getCenter();
    glm::vec3 extent = bounds.mMax;

    glm::mat4 matrix = mProjection * mView * transform;

    glm::vec4 rowX = glm::row(matrix, 0);
    glm::vec4 rowY = glm::row(matrix, 1);
    glm::vec4 rowZ = glm::row(matrix, 2);
    glm::vec4 rowW = glm::row(matrix, 3);

    std::array<glm::vec4, 6> planes;

    planes[0] = (rowW + rowX);
    planes[1] = (rowW - rowX);
    planes[2] = (rowW + rowY);
    planes[3] = (rowW - rowY);
    planes[4] = (rowW + rowZ);
    planes[5] = (rowW - rowZ);

    isInCameraView = ([&] {
        for (const auto& plane : planes)
        {
            if ((glm::dot(glm::vec3(plane), center) +
                 glm::dot(glm::abs(glm::vec3(plane)), extent)) < -plane.w)
            {
                return false;
            }
        }

        return true;
    })();
}
