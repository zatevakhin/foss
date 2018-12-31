
#ifndef FOSS_SGEOMETRYLAYOUT_HPP
#define FOSS_SGEOMETRYLAYOUT_HPP

#include "core/geometry/auxiliary.hpp"

#include <stddef.h>


namespace geometry
{

struct SGeometryLayout
{
    static const size_t UNSET = size_t(-1);

    EPrimitiveType mPrimitive = EPrimitiveType::ePoints;

    size_t mVertexCount = 0;
    size_t mIndexCount = 0;

    size_t mVertexSize = 0;
    size_t mBaseVertexOffset = 0;
    size_t mBaseIndexOffset = 0;

    size_t mPosition3D = UNSET;
    size_t mTexCoord2D = UNSET;

    size_t mNormal = UNSET;

    size_t mTangent = UNSET;
    size_t mBitangent = UNSET;
};

}

#endif //FOSS_SGEOMETRYLAYOUT_HPP
