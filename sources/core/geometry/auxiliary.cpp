
#include "auxiliary.hpp"
#include "auxiliary/opengl.hpp"

#include <stdexcept>


namespace geometry
{
    const unsigned int mapPrimitiveType(const EPrimitiveType & primitive)
    {
        switch (primitive)
        {
            case EPrimitiveType::eLines:
                return GL_LINES;
            case EPrimitiveType::ePoints:
                return GL_POINTS;
            case EPrimitiveType::eTriangleFan:
                return GL_TRIANGLE_FAN;
            case EPrimitiveType::eTriangleStrip:
                return GL_TRIANGLE_STRIP;
            case EPrimitiveType::eTriangles:
                return GL_TRIANGLES;
        }

        throw std::runtime_error("Unhandled primitive type");
    }
} // geometry