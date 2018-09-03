
#ifndef FOSS_COMPONENTS_CTRANSFORM3DCOMPONENT_HPP
#define FOSS_COMPONENTS_CTRANSFORM3DCOMPONENT_HPP

#include "core/scene/CTransform3D.hpp"

#include <anax/Component.hpp>


class CTransform3DComponent 
    : public anax::Component
    , public CTransform3D
{
};

#endif //FOSS_COMPONENTS_CTRANSFORM3DCOMPONENT_HPP
