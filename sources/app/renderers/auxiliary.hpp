#pragma once

#include "app/textures/ITexture.hpp"
#include "app/geometry/SGeometryLayout.hpp"


namespace geometry
{
    struct SGeometryLayout;
}


namespace renderers
{

void drawRangeElements(const geometry::SGeometryLayout & layout);
}
