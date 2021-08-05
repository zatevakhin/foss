
#include "Mesh.hpp"
#include "app/auxiliary/trace.hpp"


Mesh::Mesh(TVboList vbos, TBoundingBoxSharedPtr bbox, MeshInfo& meshInfo)
    : mVbos(vbos)
    , mBoundingBox(bbox)
    , mMeshInfo(meshInfo)
{
}

void Mesh::draw(TProgramAdapterPtr)
{
    glDrawElements(mMeshInfo.mode, mMeshInfo.count, mMeshInfo.type, mMeshInfo.indicies);
}
