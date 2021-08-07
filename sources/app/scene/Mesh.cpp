
#include "Mesh.hpp"
#include "app/auxiliary/trace.hpp"


Mesh::Mesh(TVaoSharedPtr vao, TVboList vbos, TBoundingBoxSharedPtr bbox, MeshInfo& meshInfo)
    : mVao(vao)
    , mVbos(vbos)
    , mBoundingBox(bbox)
    , mMeshInfo(meshInfo)
{
}

void Mesh::draw(TProgramAdapterPtr)
{
    mVao->bind();
    glDrawElements(mMeshInfo.mode, mMeshInfo.count, mMeshInfo.type, mMeshInfo.indicies);
    mVao->unbind();
}
