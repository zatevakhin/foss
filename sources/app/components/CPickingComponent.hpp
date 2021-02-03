#pragma once

#include "ecs/Component.hpp"


class CPickingComponent : public ecs::Component<CPickingComponent>
{
public:
    bool isPicked = false;
    float intersection = 0.f;
};
