
#ifndef FOSS_CVALUERANGE_HPP
#define FOSS_CVALUERANGE_HPP

#include "core/auxiliary/glm.hpp"


template <class T>
class CValueRange
{
public:
    T mMin;
    T mMax;

    CValueRange()
            : mMin(T())
            , mMax(T())
    {
    }

    CValueRange(const T &minValue, const T &maxValue)
            : mMin(minValue)
            , mMax(maxValue)
    {
        normalize();
    }

    CValueRange getUnion(const CValueRange &other) const
    {
        CValueRange result;
        result.mMin = (glm::min)(mMin, other.mMin);
        result.mMax = (glm::max)(mMax, other.mMax);

        return result;
    }

    CValueRange getIntersection(const CValueRange &other) const
    {
        CValueRange result;
        result.mMin = (glm::max)(mMin, other.mMin);
        result.mMax = (glm::min)(mMax, other.mMax);
        result.normalize();

        return result;
    }

    void normalize()
    {
        // if min > max, min = max
        mMin = (glm::min)(mMin, mMax);
    }
};


#endif //FOSS_CVALUERANGE_HPP