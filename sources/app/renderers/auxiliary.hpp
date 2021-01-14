#pragma once

#include "app/geometry/SGeometryLayout.hpp"
#include "app/textures/ITexture.hpp"


namespace geometry
{
struct SGeometryLayout;
}


namespace renderers
{

void drawRangeElements(const geometry::SGeometryLayout& layout);
}
