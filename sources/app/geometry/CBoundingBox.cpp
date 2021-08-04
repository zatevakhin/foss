
#include "CBoundingBox.hpp"
#include "app/auxiliary/glm.hpp"


CBoundingBox::CBoundingBox(const glm::vec3& lowerBounds, const glm::vec3& upperBounds)
    : mBounds(lowerBounds, upperBounds)
{
}

CBoundingBox& CBoundingBox::operator=(const CBoundingBox& box)
{
    mBounds = box.mBounds;
    return *this;
}

void CBoundingBox::unite(const CBoundingBox& other)
{
    unite(other.mBounds.mMin, other.mBounds.mMax);
}

void CBoundingBox::unite(const TBoundingBoxSharedPtr other)
{
    unite(other->mBounds.mMin, other->mBounds.mMax);
}

void CBoundingBox::unite(const glm::vec3& lowerBounds, const glm::vec3& upperBounds)
{
    mBounds = mBounds.getUnion(CValueRange<glm::vec3>(lowerBounds, upperBounds));
}

const glm::vec3 CBoundingBox::getSize() const
{
    const auto b = getBounds<glm::vec3>();
    return glm::vec3(b.mMax.x - b.mMin.x, b.mMax.y - b.mMin.y, b.mMax.z - b.mMin.z);
}

const glm::vec3 CBoundingBox::getCenter() const
{
    const auto b = getBounds<glm::vec3>();
    return glm::vec3((b.mMin.x + b.mMax.x) / 2, (b.mMin.y + b.mMax.y) / 2,
                     (b.mMin.z + b.mMax.z) / 2);
}
