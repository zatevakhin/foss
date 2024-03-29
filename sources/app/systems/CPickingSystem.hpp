#pragma once

#include "ecs/EntityManager.hpp"

#include <glm/glm.hpp>


class CPickingSystem
{
public:
    CPickingSystem(ecs::EntityManager& entityManager);
    void update(double& delta);

private:
    ecs::EntityManager& mEntityManager;
};
