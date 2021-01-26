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

    TModelPtr get_model(const std::filesystem::path path);
};
