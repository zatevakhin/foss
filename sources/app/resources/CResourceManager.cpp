#include "CResourceManager.hpp"


constexpr const char* SHADERS_DIR = "resources/shaders";


void CResourceManager::initialize()
{
    m_shader_manager.reset(new CShaderManager(SHADERS_DIR));
    m_shader_manager->initialize();

    m_model_manager.reset(new CModelManager());
    m_model_manager->initialize();

    mTextureManager.reset(new CTextureManager());
    mTextureManager->initialize();
}

std::shared_ptr<CModelManager> CResourceManager::get_model_manager() const
{
    return m_model_manager;
}

TShaderManagerPtr CResourceManager::get_shader_manager() const
{
    return m_shader_manager;
}

TTextureManagerPtr CResourceManager::getTextureManager() const
{
    return mTextureManager;
}
