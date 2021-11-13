#pragma once

#include "app/geometry/CBoundingBox.hpp"
#include "app/resources/Material.hpp"
#include "app/scene/IMesh.hpp"
#include "app/shading/IProgramAdapter.hpp"
#include <map>
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
    virtual TBoundingBoxSharedPtr getBoundingBox() const = 0;

    virtual TMeshesList& getMeshes() = 0;
    virtual TMaterialSharedPtr getMaterialById(int id) = 0;

    virtual void update(){};
    virtual ~IModel(){};
};

using TModelPtr = std::shared_ptr<IModel>;
