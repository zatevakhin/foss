
#include "CInstancedAsteroidField.hpp"
#include "app/components/CTransform3DComponent.hpp"
#include "app/components/CInstanced3dObjectComponent.hpp"


CInstancedAsteroidField::CInstancedAsteroidField(ecs::EntityManager &entityManager)
    : mEntityManager(entityManager)
    , mEntity(entityManager.createEntity())
{   
    mEntityManager.addComponent<CInstanced3dObjectComponent>(mEntity);
}

void CInstancedAsteroidField::setupModel(SStaticModel3DPtr &model)
{
    auto &mesh = mEntityManager.addComponent<CMeshComponent>(mEntity);
    mesh.mModel = model;
}

void CInstancedAsteroidField::setupTransform(glm::vec3 position, glm::vec3 scale, glm::quat orientation)
{
    auto &transform = mEntityManager.addComponent<CTransform3DComponent>(mEntity);
    transform.mOrientation = orientation;
    transform.mPosition = position;
    transform.mScale = scale;
}





