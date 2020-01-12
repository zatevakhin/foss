
#include "CSimpleGeometry.hpp"


namespace geometry
{

CSimpleGeometry::CSimpleGeometry()
    : mVerticies(EBufferType::eAttributes)
    , mIndexes(EBufferType::eIndexes)
    , mVao()
{
}

void CSimpleGeometry::bind()
{
    mVao.bind();

    mVerticies.bind();
    mIndexes.bind();
}


void CSimpleGeometry::unbind()
{
    mVao.unbind();
}

} // geometry