#pragma once

#include "app/scene/IModel.hpp"


class IModelLoader
{
public:
    virtual TModelPtr getModel() = 0;
};

using TModelLoaderPtr = std::shared_ptr<IModelLoader>;
