#pragma once

#include "app/geometry/CMeshObject.hpp"
#include "app/geometry/CSimpleGeometry.hpp"
#include "app/geometry/CTerrainFace.hpp"
#include <vector>
#include <array>


class CCubeSphere : public CMeshObject
{
    public:
        explicit CCubeSphere(int resolution);
        void buildMeshes();
        void bindGeometry();

        void bind(int i);
        void unbind(int i);

    private:
        std::vector<CTerrainFace> mCubeFaces;
        std::array<geometry::CSimpleGeometry, 6> mGeometries;
        int mResolution;
};
