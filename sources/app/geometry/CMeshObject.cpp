
#include "app/geometry/CMeshObject.hpp"


CMeshObject::CMeshObject()
    : mMeshList()
{
}

void CMeshObject::add(CMesh *mesh)
{
    mMeshList.emplace_back(mesh);
}

size_t CMeshObject::count() const
{
    return mMeshList.size();
}

CMesh &CMeshObject::create()
{
    return *mMeshList.emplace_back(new CMesh());
}

CMesh &CMeshObject::get(size_t i)
{
    return *mMeshList.at(i);
}

const CMesh &CMeshObject::get(size_t i) const
{
    return *mMeshList.at(i);
}

const geometry::CBoundingBox CMeshObject::getBoundingBox() const
{
    geometry::CBoundingBox bbox;

    for (const auto &m : mMeshList)
    {
        bbox.unite(m->getBoundingBox());
    }

    return bbox;
}
