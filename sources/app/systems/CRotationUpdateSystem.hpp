
#ifndef FOSS_CROTATIONUPDATESYSTEM_HPP
#define FOSS_CROTATIONUPDATESYSTEM_HPP

#include "ecs/EntityManager.hpp"


class CRotationUpdateSystem
{
public:
    CRotationUpdateSystem(ecs::EntityManager &entityManager);
    void update(double& delta);

private:
    ecs::EntityManager &mEntityManager;
};

#endif //FOSS_CROTATIONUPDATESYSTEM_HPP
