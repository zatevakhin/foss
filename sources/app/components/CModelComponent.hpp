#pragma once

#include "app/scene/IModel.hpp"
#include "ecs/Component.hpp"


class CModelComponent : public ecs::Component<CModelComponent>
{
public:
    CModelComponent(TModelPtr& model)
        : mModel(model)
        , mIsInView(true)
    {
    }

    TModelPtr mModel;

    bool mIsInView;
};
