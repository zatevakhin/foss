#pragma once

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

    const glm::vec3 getSize() const;
    const glm::vec3 getCenter() const;

    template<typename T>
    inline const CValueRange<T> & getBounds() const
    {
        return mBounds;
    }

private:
    CValueRange<glm::vec3> mBounds;
};

} // geometry
