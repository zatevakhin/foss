#pragma once

#include "app/scene/TBasicLight.hpp"
#include "ecs/Component.hpp"

class CLightComponent : public ecs::Component<CLightComponent>
{
public:
    CLightComponent(const TBasicLightPtr& light)
        : mLight(light)
    {
    }

    TBasicLightPtr getLight() const
    {
        return mLight;
    }

private:
    TBasicLightPtr mLight;
};
