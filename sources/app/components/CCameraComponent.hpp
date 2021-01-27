#pragma once

#include "app/scene/ICamera.hpp"
#include "ecs/Component.hpp"


class CCameraComponent : public ecs::Component<CCameraComponent>
{
public:
    CCameraComponent(const TCameraPtr& camera)
        : mCamera(camera)
    {
    }

    TCameraPtr getCamera() const
    {
        return mCamera;
    }

private:
    TCameraPtr mCamera;
};
