
#ifndef FOSS_CCULLINGSYSTEM_HPP
#define FOSS_CCULLINGSYSTEM_HPP


#include "app/components/CDrawableComponent.hpp"
#include <anax/System.hpp>
#include <glm/glm.hpp>


class CCullingSystem
    : public anax::System<anax::Requires<CDrawableComponent>>
{
public:
    CCullingSystem() = default;

    inline void setViewMatrix(const glm::mat4 & view)
    {
        mView = view;
    }

    inline void setProjectionMatrix(const glm::mat4 & projection)
    {
        mProjection = projection;
    }

    void update(double& delta);

private:
    glm::mat4 mProjection = glm::mat4(1);
    glm::mat4 mView = glm::mat4(1);
};

#endif //FOSS_CCULLINGSYSTEM_HPP
