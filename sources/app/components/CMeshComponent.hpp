
#ifndef FOSS_COMPONENTS_CMESHCOMPONENT_HPP
#define FOSS_COMPONENTS_CMESHCOMPONENT_HPP

#include "core/scene/SStaticModel3D.hpp"

#include <anax/Component.hpp>


class CMeshComponent: public anax::Component
{
public:
    enum class ECategory
    {
        eEnvironment,
        eForeground,
    };

    ECategory mCategory;
    SStaticModel3DPtr mModel;
};

#endif //FOSS_COMPONENTS_CMESHCOMPONENT_HPP