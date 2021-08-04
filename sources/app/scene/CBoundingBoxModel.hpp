#pragma once

#include "IModel.hpp"


class CBoundingBoxModel
{
public:
    CBoundingBoxModel();
    ~CBoundingBoxModel() = default;

    TModelPtr getModel() const;

private:
    void setupModel();

private:
    TModelPtr mModel;
};
