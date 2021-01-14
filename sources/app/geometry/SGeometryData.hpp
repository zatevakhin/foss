#pragma once

#include "CBoundingBox.hpp"

#include <vector>


namespace geometry
{

template <class TV, class TI> struct SGeometryData
{
    std::vector<TV> mVerticies;
    std::vector<TI> mIndices;
    CBoundingBox mBBox;

    SGeometryData() = default;

    SGeometryData(const std::vector<TV>& verticies, const std::vector<TI>& indices)
        : mVerticies(verticies)
        , mIndices(indices)
    {
    }
};

} // namespace geometry
