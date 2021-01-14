#pragma once

#include "app/textures/ITexture.hpp"
#include <unordered_map>

enum class ETextureType
{
    eTexture2D,
    eVirtualTexture2D,
    eVirtualDepthStencilTexture2D,
    eTextureCubeMap
};


class CTextureManager
{

public:
    CTextureManager();
    ~CTextureManager();

    CTextureManager(const CTextureManager&) = delete;
    CTextureManager& operator=(const CTextureManager&) = delete;

    void add(const std::string name, const ETextureType textureType);

private:
    // std::unordered_map<std::string, CTextureSharedPtr> mTextureCache;
};
