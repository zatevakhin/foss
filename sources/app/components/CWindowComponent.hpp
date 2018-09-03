
#ifndef FOSS_COMPONENTS_CWINDOWCOMPONENT_HPP
#define FOSS_COMPONENTS_CWINDOWCOMPONENT_HPP

#include "app/entities/windows/IWindow.hpp"

#include <anax/Component.hpp>


class CWindowComponent: public anax::Component
{
public:
    CWindowPtr mWindow;
};

#endif //FOSS_COMPONENTS_CWINDOWCOMPONENT_HPP
