#pragma once

#include "app/geometry/CBoundingBox.hpp"
#include "app/scene/PhongMaterial.hpp"
#include "app/shading/IProgramAdapter.hpp"
#include <memory>
#include <vector>


class IMesh
{
public:
    virtual void draw(TProgramAdapterPtr program) = 0;
    virtual geometry::CBoundingBox getBoundingBox() const = 0;
    virtual TPhongMaterialPtr getMaterial() = 0; // @TODO: move to some other interface
    virtual ~IMesh(){};
};

using TMeshPtr = std::shared_ptr<IMesh>;
using TMeshesList = std::vector<TMeshPtr>;
