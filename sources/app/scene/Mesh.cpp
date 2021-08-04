
#include "Mesh.hpp"
#include "app/auxiliary/trace.hpp"


Mesh::Mesh(TVboList vbos, TBoundingBoxSharedPtr bbox, GLenum mode, GLsizei count, GLenum type,
           const void* indicies, const int materialId)
    : mVbos(vbos)
    , mMode(mode)
    , mCount(count)
    , mType(type)
    , mIndicies(indicies)
    , mBoundingBox(bbox)
    , mMaterialId(materialId)
{
}

void Mesh::draw(TProgramAdapterPtr)
{
    glDrawElements(mMode, mCount, mType, mIndicies);
}
