#pragma once

#include <memory>

class IMaterial
{
public:
    virtual ~IMaterial(){};
    virtual bool isPbr() const = 0;
};

using TMaterialPtr = std::shared_ptr<IMaterial>;
