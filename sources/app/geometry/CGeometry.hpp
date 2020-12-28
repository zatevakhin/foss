#pragma once

#include "app/scene/CVertexBufferObject.hpp"
#include "app/scene/CVertexArrayObject.hpp"
#include "auxiliary.hpp"

#include "SGeometryData.hpp"
#include "CBoundingBox.hpp"
#include "app/auxiliary/glm.hpp"

#include <boost/noncopyable.hpp>
#include <memory>
#include <vector>


namespace geometry
{

class CGeometry
{
public:
    CGeometry();
    ~CGeometry() = default;

    CGeometry(const CGeometry&) = delete;

    template<class TV, class TI>
    void copy(const SGeometryData<TV, TI> & data)
    {
        mVao.bind();
        mVerticies.copy(data.mVerticies);
        mIndexes.copy(data.mIndexes);
        mBBox = data.mBBox;
        mVao.unbind();
    }

    const CBoundingBox & getBoundingBox() const;

    void bind();
    void unbind();

private:
    CVertexBufferObject mVerticies;
    CVertexBufferObject mIndexes;
    CBoundingBox  mBBox;
    CVertexArrayObject mVao;

};

using CGeometrySharedPtr = std::shared_ptr<CGeometry>;

} // geometry
