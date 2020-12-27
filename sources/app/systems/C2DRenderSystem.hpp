
#ifndef FOSS_C2DRENDERSYSTEM_HPP
#define FOSS_C2DRENDERSYSTEM_HPP

#include "app/auxiliary/glm.hpp"


#include "ecs/EntityManager.hpp"


class C2DRenderSystem
{
public:
        C2DRenderSystem(ecs::EntityManager &entityManager);
        void render(const glm::mat4 & view, const glm::mat4 & projection);
private:
    ecs::EntityManager &mEntityManager;

};

#endif //FOSS_C2DRENDERSYSTEM_HPP
