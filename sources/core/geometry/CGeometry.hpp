
#ifndef FOSS_CGEOMETRYOBJECT_HPP
#define FOSS_CGEOMETRYOBJECT_HPP

#include "core/scene/CBufferObject.hpp"
#include "core/geometry/auxiliary.hpp"

#include "CBoundingBox.hpp"
#include "core/auxiliary/glm.hpp"

#include <boost/noncopyable.hpp>
#include <memory>
#include <vector>


namespace geometry
{

template <class TV, class TI>
struct SGeometryData
{
    std::vector<TV> mVerticies;
    std::vector<TI> mIndexes;
    CBoundingBox    mBBox;

    SGeometryData() = default;

    SGeometryData(const std::vector<TV> &verticies, const std::vector<TI> &indexes)
        : mVerticies(verticies), mIndexes(indexes)
    {
    }
};


struct SGeometryLayout
{
    static const size_t UNSET = size_t(-1);

    geometry::EPrimitiveType mPrimitive = geometry::EPrimitiveType::ePoints;

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


class CGeometry : private boost::noncopyable
{
public:
    CGeometry();

    template<class TV, class TI>
    void copy(const SGeometryData<TV, TI> & data)
    {
        mVerticies.copy(data.mVerticies);
        mIndexes.copy(data.mIndexes);
        mBBox = data.mBBox;
    }

    void bind();

private:
    CBufferObject mVerticies;
    CBufferObject mIndexes;
    CBoundingBox  mBBox;

};

using CGeometrySharedPtr = std::shared_ptr<CGeometry>;

} // geometry

#endif //FOSS_CGEOMETRYOBJECT_HPP
