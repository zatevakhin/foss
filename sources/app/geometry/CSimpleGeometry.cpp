
#include "CSimpleGeometry.hpp"


namespace geometry
{

CSimpleGeometry::CSimpleGeometry()
    : mVerticies(EBufferType::eAttributes)
    , mIndexes(EBufferType::eIndexes)
    , mVao()
{
}

void CSimpleGeometry::bind() const
{
    mVao.bind();
    // mVerticies.bind();
    // mIndexes.bind();
}


void CSimpleGeometry::unbind() const
{
    // mIndexes.unbind();
    // mVerticies.unbind();
    mVao.unbind();
}

} // geometry