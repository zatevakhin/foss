#pragma once

#include "ecs/EntityManager.hpp"


class CRotationUpdateSystem
{
public:
    CRotationUpdateSystem(ecs::EntityManager &entityManager);
    void update(double& delta);

private:
    ecs::EntityManager &mEntityManager;
};
