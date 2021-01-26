#pragma once

#include "app/scene/CTransform3D.hpp"
#include "ecs/Component.hpp"

class CTransform3DComponent
    : public ecs::Component<CTransform3DComponent>
    , public CTransform3D
{
};
