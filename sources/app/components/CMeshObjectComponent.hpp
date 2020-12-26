#pragma once

#include "app/base/geometry/CMeshObject.hpp"
#include "ecs/Component.hpp"


class CMeshObjectComponent: public ecs::Component<CMeshObjectComponent>
{
public:
    CMeshObjectPtr mMeshObject;
};

