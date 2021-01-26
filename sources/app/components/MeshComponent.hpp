#pragma once

#include "app/scene/IMesh.hpp"
#include "ecs/Component.hpp"


class MeshComponent : public ecs::Component<MeshComponent>
{
public:
    TMeshPtr mMesh;
};
