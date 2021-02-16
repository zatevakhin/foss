#pragma once

#include "CModelManager.hpp"
#include "CShaderManager.hpp"
#include "CTextureManager.hpp"

#include "app/SEngineSettings.hpp"

#include <memory>


class CResourceManager
{

public:
    CResourceManager() = default;
    ~CResourceManager() = default;

    CResourceManager(const CResourceManager&) = delete;
    CResourceManager& operator=(const CResourceManager&) = delete;

    void initialize();

    std::shared_ptr<CModelManager> get_model_manager() const;
    std::shared_ptr<CShaderManager> get_shader_manager() const;
    std::shared_ptr<CTextureManager> getTextureManager() const;

private:
    std::shared_ptr<CModelManager> m_model_manager;
    std::shared_ptr<CShaderManager> m_shader_manager;
    std::shared_ptr<CTextureManager> mTextureManager;
};
