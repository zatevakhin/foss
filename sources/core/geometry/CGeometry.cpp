
#include "CGeometry.hpp"


namespace geometry
{

CGeometry::CGeometry()
    : mVerticies(eBufferType::Attributes)
    , mIndexes(eBufferType::Indexes)
{
}

void CGeometry::bind()
{
    mVerticies.bind();
    mIndexes.bind();
}

} // geometry