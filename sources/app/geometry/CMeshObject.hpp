#pragma once

#include "app/geometry/CMesh.hpp"
#include <memory>

class CMeshObject
{

public:
    CMeshObject();
    size_t count() const;
    CMesh& create();
    CMesh& get(size_t i);
    const CMesh& get(size_t i) const;

    const geometry::CBoundingBox getBoundingBox() const;

private:
    std::vector<std::unique_ptr<CMesh>> mMeshList;

};

using CMeshObjectPtr = std::shared_ptr<CMeshObject>;
