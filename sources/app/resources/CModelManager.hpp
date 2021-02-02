#pragma once

#include "app/scene/IModel.hpp"
#include <filesystem>


class CModelManager
{
public:
    CModelManager();
    ~CModelManager();

    CModelManager(const CModelManager&) = delete;
    CModelManager& operator=(const CModelManager&) = delete;

    TModelPtr getModel(const std::filesystem::path path);
};
