#pragma once

#include "app/geometry/CBoundingBox.hpp"
#include "app/shading/IProgramAdapter.hpp"
#include <memory>


enum class EModelType
{
    STATIC,
    DYNAMIC,
    SKELETAL,
};


class IModel
{
public:
    virtual EModelType getType() const = 0;
    virtual void draw(TProgramAdapterPtr program) = 0;
    virtual geometry::CBoundingBox getBoundingBox() const = 0;

    virtual void update() {};
    virtual ~IModel() {};
};

using TModelPtr = std::shared_ptr<IModel>;
