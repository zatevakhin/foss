#pragma once

#include "IModel.hpp"


class CBoundingBox
{
public:
    CBoundingBox();
    ~CBoundingBox() = default;

    TModelPtr getModel() const;

private:
    void setupModel();

private:
    TModelPtr mModel;
};
