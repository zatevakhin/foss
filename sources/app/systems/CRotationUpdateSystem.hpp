
#ifndef FOSS_CROTATIONUPDATESYSTEM_HPP
#define FOSS_CROTATIONUPDATESYSTEM_HPP

#include "app/components/CTransform3DComponent.hpp"
#include <anax/System.hpp>

class CBoundingBox3DRenderer;


class CRotationUpdateSystem
    : public anax::System<anax::Requires<CTransform3DComponent>>
{
public:
    CRotationUpdateSystem();
    void update(double& delta);
};

#endif //FOSS_CROTATIONUPDATESYSTEM_HPP
