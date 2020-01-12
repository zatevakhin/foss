
#ifndef FOSS_ENTITIES_INSTANCED_ASTEROID_FIELD_HPP
#define FOSS_ENTITIES_INSTANCED_ASTEROID_FIELD_HPP


#include "app/components/CMeshComponent.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/Entity.hpp"


class CInstancedAsteroidField
{

public:
    CInstancedAsteroidField(ecs::EntityManager &entityManager);
    ~CInstancedAsteroidField() = default;

    void setupModel(SStaticModel3DPtr &model);
    void setupTransform(glm::vec3 position, glm::vec3 scale, glm::quat orientation);

private:
    ecs::EntityManager &mEntityManager;
    ecs::Entity mEntity;
};


#endif //FOSS_ENTITIES_INSTANCED_ASTEROID_FIELD_HPP
