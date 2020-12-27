
#include "CGeometry.hpp"


namespace geometry
{

CGeometry::CGeometry()
    : mVerticies(EBufferType::eAttributes)
    , mIndexes(EBufferType::eIndexes)
    , mVao()
{
}

const CBoundingBox & CGeometry::getBoundingBox() const
{
    return mBBox;
}

void CGeometry::bind()
{
    mVao.bind();
    mVerticies.bind();
    mIndexes.bind();
}


void CGeometry::unbind()
{
    mVao.unbind();
}


} // geometry