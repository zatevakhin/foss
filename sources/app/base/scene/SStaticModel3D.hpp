
#ifndef FOSS_SSTATICMODEL3D_HPP
#define FOSS_SSTATICMODEL3D_HPP

#include "app/base/geometry/CGeometry.hpp"
#include "app/base/geometry/SGeometryLayout.hpp"
#include "app/base/resources/SPhongMaterial.hpp"
#include "app/base/auxiliary/glm.hpp"

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

#endif //FOSS_SSTATICMODEL3D_HPP
