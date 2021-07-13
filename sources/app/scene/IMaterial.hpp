#pragma once

#include <memory>
#include <vector>

class IMaterial
{
public:
    virtual ~IMaterial(){};
};

using TMaterialPtr = std::shared_ptr<IMaterial>;
using TMaterialList = std::vector<TMaterialPtr>;
