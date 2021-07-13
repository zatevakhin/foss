#pragma once

#include "app/auxiliary/trace.hpp"
#include "app/textures/ITexture.hpp"

#include <filesystem>
#include <fmt/core.h>
#include <string>
#include <unordered_map>

enum class ETextureType
{
    VIRTUAL,
    FILE
};


using TTextureMap = std::unordered_map<std::string, TTextureSharedPtr>;

class CTextureManager
{

public:
    CTextureManager() = default;
    ~CTextureManager() = default;

    CTextureManager(const CTextureManager&) = delete;
    CTextureManager& operator=(const CTextureManager&) = delete;

    void initialize(){};

    template <class T, typename = std::enable_if_t<std::is_base_of<ITexture, T>::value>>
    std::shared_ptr<T> create(const std::string path, ETextureType type)
    {
        std::string name = std::filesystem::path(path).filename();
        TTextureSharedPtr texturePtr(nullptr);

        spdlog::debug("Create texture<{}>: {}", type, name);

        if (mTextureCache.find(name) == mTextureCache.end())
        {
            mTextureCache.emplace(name, new T());
            texturePtr = mTextureCache[name];

            if (ETextureType::FILE == type)
            {
                getTexture<T>(path, std::static_pointer_cast<T>(texturePtr));
            }
        }

        return std::static_pointer_cast<T>(mTextureCache[name]);
    }

    template <class T>
    std::shared_ptr<T> get(const std::string name)
    {
        return std::static_pointer_cast<T>(mTextureCache[name]);
    }

    template <class T>
    static void getTexture(const std::filesystem::path path, std::shared_ptr<T> texture);

    const TTextureMap& getTexturesList() const
    {
        return mTextureCache;
    }


private:
    TTextureMap mTextureCache;
};

using TTextureManagerPtr = std::shared_ptr<CTextureManager>;
