#pragma once

#include "app/scene/IModel.hpp"
#include "ecs/Component.hpp"


struct SModelDebug
{
    enum class EDebugDrawMode
    {
        FILL,
        LINE,
        POINT
    };

    bool mHideModel{false};
    bool mHideBox{false};
    bool mEnableDebugDraw{false};
    bool mEnableNormalsDraw{false};

    EDebugDrawMode mDebugDrawMode{EDebugDrawMode::FILL};
};

class CModelComponent : public ecs::Component<CModelComponent>
{
public:
    CModelComponent(TModelPtr& model)
        : mModel(model)
        , mIsInView(true)
    {
    }

    SModelDebug mDebug;
    TModelPtr mModel;

    bool mIsInView;
};
