
#ifndef FOSS_PICKING_SYSTEM_HPP
#define FOSS_PICKING_SYSTEM_HPP

#include "app/components/C3dObjectComponent.hpp"
#include "ecs/EntityManager.hpp"

#include <glm/glm.hpp>


class CPickingSystem
{
public:
    CPickingSystem(ecs::EntityManager &entityManager);
    void update(double& delta);

private:
    ecs::EntityManager &mEntityManager;
};

#endif //FOSS_PICKING_SYSTEM_HPP
