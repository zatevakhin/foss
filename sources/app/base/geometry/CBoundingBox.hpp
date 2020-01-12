
#ifndef FOSS_CBOUNDINGBOX_HPP
#define FOSS_CBOUNDINGBOX_HPP

#include "CValueRange.hpp"

namespace geometry
{

// AABB
class CBoundingBox
{
public:
    CBoundingBox() = default;
    explicit CBoundingBox(const glm::vec3 &lowerBounds, const glm::vec3 & upperBounds);

    void unite(const CBoundingBox & other);
    void unite(const glm::vec3 & lowerBounds, const glm::vec3 & upperBounds);

    template<typename T>
    inline const CValueRange<T> & getBounds() const
    {
        return mBounds;
    }

private:
    CValueRange<glm::vec3> mBounds;
};

} // geometry

#endif //FOSS_CBOUNDINGBOX_HPP
