
#ifndef FOSS_CSIMPLEGEOMETRY_HPP
#define FOSS_CSIMPLEGEOMETRY_HPP

#include "core/scene/CBufferObject.hpp"
#include "core/scene/CArrayObject.hpp"
#include "core/geometry/auxiliary.hpp"
#include "SGeometryData.hpp"

#include <boost/noncopyable.hpp>
#include <memory>


namespace geometry
{

class CSimpleGeometry : private boost::noncopyable
{
public:
    CSimpleGeometry();
    ~CSimpleGeometry() = default;

    template<class TV, class TI>
    void copy(const std::vector<TV> & data, const std::vector<TI> & indexes)
    {
        mVao.bind();
        mVerticies.copy(data);
        mIndexes.copy(indexes);
        mVao.unbind();
    }

    void bind();
    void unbind();

private:
    CBufferObject mVerticies;
    CBufferObject mIndexes;
    CArrayObject  mVao;
};

using CSimpleGeometrySharedPtr = std::shared_ptr<CSimpleGeometry>;

} // geometry

#endif //FOSS_CSIMPLEGEOMETRY_HPP
