#pragma once

#include <memory>
#include <vector>

class IMaterial
{
public:
    virtual ~IMaterial(){};
    virtual bool isPbr() const = 0;
};

using TMaterialPtr = std::shared_ptr<IMaterial>;
using TMaterialList = std::vector<TMaterialPtr>;
