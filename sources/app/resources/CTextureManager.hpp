#pragma once

#include "app/textures/CTexture2D.hpp"
#include "app/textures/CTextureCubeMap.hpp"
#include "app/textures/ITexture.hpp"

#include <filesystem>
#include <fmt/core.h>
#include <string>
#include <unordered_map>

namespace TextureManagement
{

TTextureSharedPtr getTexture2D(const std::filesystem::path path);
TTextureSharedPtr getTextureCubeMap(const std::filesystem::path path);

} // namespace TextureManagement


class CTextureManager
{

public:
    enum class Type
    {
        VIRTUAL,
        FILE
    };

    CTextureManager();
    ~CTextureManager() = default;

    CTextureManager(const CTextureManager&) = delete;
    CTextureManager& operator=(const CTextureManager&) = delete;

    void initialize(){};

    template <class T>
    std::shared_ptr<T> create(const std::string name, Type type)
    {
        static_assert(std::is_base_of<ITexture, T>::value, "Should be derived from ITexture!");

        if (mTextureCache.find(name) == mTextureCache.end())
        {
            if (type == Type::FILE)
            {
                if (std::is_same<T, CTextureCubeMap>::value)
                {
                    mTextureCache.emplace(name, TextureManagement::getTextureCubeMap(name));
                }
                else
                {
                    mTextureCache.emplace(name, TextureManagement::getTexture2D(name));
                }
            }
            else
            {
                mTextureCache.emplace(name, new T());
            }
        }

        return std::static_pointer_cast<T>(mTextureCache[name]);
    }

    template <class T>
    std::shared_ptr<T> get(const std::string name)
    {
        return std::static_pointer_cast<T>(mTextureCache[name]);
    }

private:
    std::unordered_map<std::string, TTextureSharedPtr> mTextureCache;
};
