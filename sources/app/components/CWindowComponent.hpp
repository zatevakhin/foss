#pragma once

#include "app/entities/windows/IWindow.hpp"
#include "ecs/Component.hpp"


class CWindowComponent: public ecs::Component<CWindowComponent>
{
public:
    CWindowPtr mWindow;
};
