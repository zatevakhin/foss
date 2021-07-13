#pragma once

#include "app/geometry/CBoundingBox.hpp"
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
    virtual geometry::CBoundingBox getBoundingBox() const = 0;
    // @TODO: move to some other interface
    // mesh access etc.
    virtual TMeshesList& getMeshes() = 0;
    virtual TMaterialList& getMaterials() = 0;
    virtual TMaterialList getMaterials(TMeshPtr mesh) = 0;

    virtual void update(){};
    virtual ~IModel(){};
};

using TModelPtr = std::shared_ptr<IModel>;

// many materials to many meshes
using TMaterialToMeshMap = std::vector<std::pair<TMeshPtr, TMaterialPtr>>;
