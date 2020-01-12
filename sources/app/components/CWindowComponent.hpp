
#ifndef FOSS_COMPONENTS_CWINDOWCOMPONENT_HPP
#define FOSS_COMPONENTS_CWINDOWCOMPONENT_HPP

#include "app/entities/windows/IWindow.hpp"
#include "ecs/Component.hpp"


class CWindowComponent: public ecs::Component<CWindowComponent>
{
public:
    CWindowPtr mWindow;
};

#endif //FOSS_COMPONENTS_CWINDOWCOMPONENT_HPP
