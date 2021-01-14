#pragma once

#include "app/auxiliary/glm.hpp"
#include "app/scene/CVertexArrayObject.hpp"
#include "app/scene/CVertexBufferObject.hpp"
#include "auxiliary.hpp"

#include "CBoundingBox.hpp"
#include "SGeometryData.hpp"

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

    template <class TV, class TI> void copy(const SGeometryData<TV, TI>& data)
    {
        mVao.bind();
        mVerticies.copy(data.mVerticies);
        mIndices.copy(data.mIndices);
        mBBox = data.mBBox;
        mVao.unbind();
    }

    const CBoundingBox& getBoundingBox() const;

    void bind();
    void unbind();

private:
    CVertexBufferObject mVerticies;
    CVertexBufferObject mIndices;
    CBoundingBox mBBox;
    CVertexArrayObject mVao;
};

using CGeometrySharedPtr = std::shared_ptr<CGeometry>;

} // namespace geometry
