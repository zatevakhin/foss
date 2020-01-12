
#ifndef FOSS_COMPONENTS_CMESHCOMPONENT_HPP
#define FOSS_COMPONENTS_CMESHCOMPONENT_HPP

#include "app/base/scene/SStaticModel3D.hpp"

#include "ecs/Component.hpp"


class CMeshComponent: public ecs::Component<CMeshComponent>
{
public:
    SStaticModel3DPtr mModel;
};

#endif //FOSS_COMPONENTS_CMESHCOMPONENT_HPP
