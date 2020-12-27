
#include "app/geometry/CMeshObject.hpp"


CMeshObject::CMeshObject()
    : mMeshList()
{
}

size_t CMeshObject::count() const
{
    return mMeshList.size();
}

CMesh& CMeshObject::create()
{
    return *mMeshList.emplace_back(std::make_unique<CMesh>());
}

CMesh& CMeshObject::get(size_t i)
{
    return *mMeshList.at(i);
}

const CMesh& CMeshObject::get(size_t i) const
{
    return *mMeshList.at(i);
}
