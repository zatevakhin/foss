#pragma once

#include "app/geometry/CMeshObject.hpp"
#include "app/geometry/CTerrainFace.hpp"
#include <array>
#include <vector>

class CCubeSphere : public CMeshObject
{
public:
    explicit CCubeSphere(int resolution);
    void bindGeometry();

    void bind(size_t i) const;
    void unbind(size_t i) const;

private:
    std::vector<CTerrainFace> mCubeFaces;
    int mResolution;
};
