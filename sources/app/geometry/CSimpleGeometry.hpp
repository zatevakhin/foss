#pragma once

#include "app/scene/CVertexBufferObject.hpp"
#include "app/scene/CVertexArrayObject.hpp"
#include "auxiliary.hpp"
#include "SGeometryData.hpp"

#include <memory>


namespace geometry
{

class CSimpleGeometry
{
public:
    CSimpleGeometry();
    ~CSimpleGeometry() = default;

    CSimpleGeometry(const CSimpleGeometry&) = delete;

    template<class TV, class TI>
    void copy(const std::vector<TV> & data, const std::vector<TI> & indexes)
    {
        mVao.bind();
        mVerticies.copy(data);
        mIndexes.copy(indexes);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        mVao.unbind();
    }

    void bind() const;
    void unbind() const;

private:
    CVertexBufferObject mVerticies;
    CVertexBufferObject mIndexes;
    CVertexArrayObject mVao;
};

using CSimpleGeometrySharedPtr = std::shared_ptr<CSimpleGeometry>;

} // geometry
