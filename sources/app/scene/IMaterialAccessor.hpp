#pragma once

#include "CPbrMaterial.hpp"
#include "CPhongMaterial.hpp"

#include <vector>


class IMaterialAccessor
{
public:
    virtual ~IMaterialAccessor(){};

    virtual TPhongMaterialPtr getPhongMaterial() const = 0;
    virtual TPbrMaterialPtr getPbrMaterial() const = 0;
};

using TMaterialAccessorList = std::vector<IMaterialAccessor>;
