#pragma once

#include "app/geometry/CBoundingBox.hpp"
#include "app/shading/IProgramAdapter.hpp"
#include <memory>
#include <vector>


//@todo: remove materials from mesh
// matterials should be in model and mapped to meshes.

class IMesh
{
public:
    virtual void draw(TProgramAdapterPtr program) = 0;
    virtual geometry::CBoundingBox getBoundingBox() const = 0;

    virtual ~IMesh(){};
};

using TMeshPtr = std::shared_ptr<IMesh>;
using TMeshesList = std::vector<TMeshPtr>;
