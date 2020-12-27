#pragma once

#include "app/geometry/CGeometry.hpp"
#include "app/geometry/SGeometryLayout.hpp"
#include "app/resources/SPhongMaterial.hpp"
#include "app/auxiliary/glm.hpp"

#include <memory>

struct SStaticModel3D;
using SStaticModel3DPtr = std::shared_ptr<SStaticModel3D>;


class CStaticMesh3D
{
public:
    geometry::SGeometryLayout mLayout;
    glm::mat4 mLocal;
    unsigned int mMaterialIndex = 0U;
};

struct SStaticModel3D
{
    SStaticModel3D()
    {
    }

    std::vector<SPhongMaterial>  mMaterials;
    std::vector<CStaticMesh3D>   mMeshes;
    geometry::CGeometrySharedPtr mGeometry;
};
