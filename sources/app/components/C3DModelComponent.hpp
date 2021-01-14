#pragma once

#include "app/scene/SStaticModel3D.hpp"
#include "ecs/Component.hpp"


class C3DModelComponent : public ecs::Component<C3DModelComponent>
{
public:
    SStaticModel3DPtr mModel;
};
