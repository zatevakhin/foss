#pragma once

#include "IModel.hpp"


class CCubeSphere
{
public:
    explicit CCubeSphere(int resolution);
    TModelPtr getModel() const;

private:
    int mResolution;
};
