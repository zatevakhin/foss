
#ifndef FOSS_C2DRENDERSYSTEM_HPP
#define FOSS_C2DRENDERSYSTEM_HPP

#include "app/components/CWindowComponent.hpp"
#include "core/auxiliary/glm.hpp"

#include <anax/System.hpp>

class CWindowComponent;


class C2DRenderSystem
    : public anax::System<anax::Requires<CWindowComponent>>
{
    public:
        C2DRenderSystem() = default;
        void render(const glm::mat4 & view, const glm::mat4 & projection);
};


#endif //FOSS_C2DRENDERSYSTEM_HPP