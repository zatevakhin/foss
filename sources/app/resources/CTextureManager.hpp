#pragma once

#include "app/textures/ITexture.hpp"
#include <string>
#include <unordered_map>


class CTextureManager
{

public:
    CTextureManager() = default;
    ~CTextureManager() = default;

    CTextureManager(const CTextureManager&) = delete;
    CTextureManager& operator=(const CTextureManager&) = delete;

    void add(const std::string name);

private:
    // std::unordered_map<std::string, TTextureSharedPtr> mTextureCache;
};
