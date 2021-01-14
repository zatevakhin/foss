#pragma once

#include "ecs/Component.hpp"


class C3dObjectComponent : public ecs::Component<C3dObjectComponent>
{
public:
    bool isInCameraView = false;
    bool isInstanced = false;
    bool isPicked = false;

    float intersection = 0.f;
};
