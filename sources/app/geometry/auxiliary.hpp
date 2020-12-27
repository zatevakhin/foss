#pragma once


namespace geometry
{

enum class EPrimitiveType
{
    ePoints,
    eLines,
    eTriangles,
    eTriangleFan,
    eTriangleStrip,
};

const unsigned int mapPrimitiveType(const EPrimitiveType & primitive);

} // geometry
