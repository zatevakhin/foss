
#include "CBoundingBox.hpp"

#include "app/base/auxiliary/glm.hpp"


namespace geometry
{

CBoundingBox::CBoundingBox(const glm::vec3 & lowerBounds, const glm::vec3 & upperBounds)
        : mBounds(lowerBounds, upperBounds)
{
}

void CBoundingBox::unite(const CBoundingBox & other)
{
    unite(other.mBounds.mMin, other.mBounds.mMax);
}

void CBoundingBox::unite(const glm::vec3 & lowerBounds, const glm::vec3 & upperBounds)
{
    mBounds = mBounds.getUnion(CValueRange<glm::vec3>(lowerBounds, upperBounds));
}

} // geometry
