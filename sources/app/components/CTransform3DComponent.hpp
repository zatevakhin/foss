
#ifndef FOSS_COMPONENTS_CTRANSFORM3DCOMPONENT_HPP
#define FOSS_COMPONENTS_CTRANSFORM3DCOMPONENT_HPP

#include "app/scene/CTransform3D.hpp"

#include "ecs/Component.hpp"


class CTransform3DComponent
    : public ecs::Component<CTransform3DComponent>
    , public CTransform3D
{
};

#endif //FOSS_COMPONENTS_CTRANSFORM3DCOMPONENT_HPP
