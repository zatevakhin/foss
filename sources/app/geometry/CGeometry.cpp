
#include "CGeometry.hpp"


namespace geometry
{

CGeometry::CGeometry()
    : mVerticies(EBufferType::eArrayBuffer)
    , mIndices(EBufferType::eElementArrayBuffer)
    , mVao()
{
}

const CBoundingBox& CGeometry::getBoundingBox() const
{
    return mBBox;
}

void CGeometry::bind()
{
    mVao.bind();
    mVerticies.bind();
    mIndices.bind();
}


void CGeometry::unbind()
{
    mVao.unbind();
}


} // namespace geometry
