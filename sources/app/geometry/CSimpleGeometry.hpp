#pragma once

#include "app/scene/CBufferObject.hpp"
#include "app/scene/CArrayObject.hpp"
#include "auxiliary.hpp"
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

    void bind() const;
    void unbind() const;

private:
    CBufferObject mVerticies;
    CBufferObject mIndexes;
    CArrayObject  mVao;
};

using CSimpleGeometrySharedPtr = std::shared_ptr<CSimpleGeometry>;

} // geometry
