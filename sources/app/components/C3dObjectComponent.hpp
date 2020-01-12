
#ifndef FOSS_COMPONENTS_DRAWABLE_COMPONENT_HPP
#define FOSS_COMPONENTS_DRAWABLE_COMPONENT_HPP


#include "ecs/Component.hpp"


class C3dObjectComponent: public ecs::Component<C3dObjectComponent>
{
public:

    bool isInCameraView = false;
    bool isInstanced = false;
};

#endif //FOSS_COMPONENTS_DRAWABLE_COMPONENT_HPP
