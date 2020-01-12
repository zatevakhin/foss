#ifndef FOSS_CORE_RENDERERS_AUXILIARY_HPP
#define FOSS_CORE_RENDERERS_AUXILIARY_HPP

#include "app/base/textures/ITexture.hpp"
#include "app/base/geometry/SGeometryLayout.hpp"


namespace geometry
{
    struct SGeometryLayout;
}


namespace renderers
{

void drawRangeElements(const geometry::SGeometryLayout & layout);
}

#endif //FOSS_CORE_RENDERERS_AUXILIARY_HPP
