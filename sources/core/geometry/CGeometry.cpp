
#include "CGeometry.hpp"


namespace geometry
{

CGeometry::CGeometry()
    : mVerticies(EBufferType::eAttributes)
    , mIndexes(EBufferType::eIndexes)
{
}

void CGeometry::bind()
{
    mVerticies.bind();
    mIndexes.bind();
}

} // geometry