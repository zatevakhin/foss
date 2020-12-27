
#ifndef FOSS_GEOMETRY_AUXILIARY_HPP
#define FOSS_GEOMETRY_AUXILIARY_HPP


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

#endif //FOSS_GEOMETRY_AUXILIARY_HPP
