
#include "auxiliary.hpp"
#include "app/base/auxiliary/opengl.hpp"
#include "app/base/geometry/CGeometry.hpp"


namespace renderers
{

void drawRangeElements(const geometry::SGeometryLayout & layout)
{
    const GLenum primitive = geometry::mapPrimitiveType(layout.mPrimitive);
    const GLuint start = 0u;
    const GLuint end = unsigned(layout.mVertexCount);
    const GLsizei count = GLsizei(layout.mIndexCount);

    const void *offset = reinterpret_cast<const void*>(layout.mBaseIndexOffset);

    glDrawRangeElements(primitive, start, end, count, GL_UNSIGNED_INT, reinterpret_cast<const void*>(offset));
}

} // renders