#pragma once

#include "IModelLoader.hpp"
#include "app/scene/IModel.hpp"
#include <filesystem>


class CModelManager
{
public:
    CModelManager() = default;
    ~CModelManager() = default;

    CModelManager(const CModelManager&) = delete;
    CModelManager& operator=(const CModelManager&) = delete;

    void initialize();

    TModelPtr getModel();
};
