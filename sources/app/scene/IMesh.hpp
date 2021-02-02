#pragma once

#include "app/geometry/CBoundingBox.hpp"
#include "app/shading/IShaderProgram.hpp"
#include <memory>


class IMesh
{
public:
    virtual void draw(TProgramSharedPtr program) = 0;
    virtual geometry::CBoundingBox getBoundingBox() const = 0;
    virtual ~IMesh(){};
};

using TMeshPtr = std::shared_ptr<IMesh>;
