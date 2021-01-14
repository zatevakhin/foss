
#include "CResourceLoader.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/sdl.hpp"
#include "app/textures/CTexture2D.hpp"
#include "app/textures/CTextureCubeMap.hpp"
#include "nlohmann/json.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <sstream>

using json = nlohmann::json;


CResourceLoader::CResourceLoader()
{
}


CResourceLoader::~CResourceLoader()
{
}


std::string CResourceLoader::getFileAsString(const fs::path& path)
{
    const fs::path absolute = CResourceLoader::getAbsolute(path);
    std::ifstream f;

    f.open(path.native());
    if (!f.is_open())
    {
        throw std::runtime_error("Cannot open for reading: " + absolute.generic_string());
    }

    std::stringstream ss;
    ss << f.rdbuf();

    return ss.str();
}


CTextureSharedPtr CResourceLoader::getTexture(const fs::path& path)
{
    const fs::path abspath = CResourceLoader::getAbsolute(path);
    CTextureSharedPtr texture = nullptr; // mTextureCache.get(abspath);

    if (!texture)
    {
        texture = loadTexture(abspath);
        // mTextureCache.add(abspath, texture);
    }
    return texture;
}

CTextureSharedPtr CResourceLoader::getCubeMap(const fs::path& path, unsigned int size)
{
    const fs::path abspath = CResourceLoader::getAbsolute(path);
    CTextureSharedPtr texture = nullptr; // mTextureCache.get(abspath);

    if (!texture)
    {
        texture = loadCubeMap(abspath, size);
        // mTextureCache.add(abspath, texture);
    }
    return texture;
}


TSurfacePtr CResourceLoader::loadImage(const fs::path& path, bool& hasAlpha)
{
    const fs::path abspath = CResourceLoader::getAbsolute(path);
    TSurfacePtr surface(IMG_Load(abspath.native().c_str()));

    if (!surface)
    {
        throw std::runtime_error("Cannot find texture at " + path.generic_string());
    }

    hasAlpha = SDL_ISPIXELFORMAT_ALPHA(surface->format->format);

    // All images will be converted to RGB or RGBA (if has alpha chanel).
    const uint32_t requiredFormat = hasAlpha ? SDL_PIXELFORMAT_ABGR8888 : SDL_PIXELFORMAT_RGB24;

    if (surface->format->format != requiredFormat)
    {
        surface.reset(SDL_ConvertSurfaceFormat(surface.get(), requiredFormat, 0));
    }

    return surface;
}

fs::path CResourceLoader::getAbsolute(const fs::path& path)
{
    if (path.is_absolute())
    {
        return path;
    }

    const auto abspath = fs::absolute(path, fs::current_path());
    if (fs::exists(abspath))
    {
        return abspath;
    }

    throw std::runtime_error("Resource not found: " + path.generic_string());
}


CTextureSharedPtr CResourceLoader::loadTexture(const fs::path& path)
{
    bool hasAlpha;
    TSurfacePtr surface = loadImage(path, hasAlpha);

    auto texture = std::make_shared<CTexture2D>();
    texture->bind();

    //    texture->ApplyTrilinearFilter();
    //    texture->ApplyMaxAnisotropy();
    texture->setWrapMode(textures::ETextureWrapMode::eRepeat, textures::ETextureWrapMode::eRepeat);

    texture->setFilter();

    const auto size = glm::ivec2(surface->w, surface->h);

    const GLenum format = hasAlpha ? GL_RGBA : GL_RGB;
    texture->setTexture(format, format, GL_UNSIGNED_BYTE, size, surface->pixels);
    texture->generateMipMaps();
    CTexture2D::unbind();

    return texture;
}

CTextureSharedPtr CResourceLoader::loadCubeMap(const fs::path& path, unsigned int size)
{
    fs::path cubeMap = CResourceLoader::getAbsolute(path);
    cubeMap /= "skyboxset.json";

    std::ifstream cubeMapFile(cubeMap.native());
    json cubeMapData;
    cubeMapFile >> cubeMapData;

    //    if (cubeMapData["sizes"].find(size))
    //    {
    //        throw std::invalid_argument(fmt::format("This cubemap not have size '%d'!", size));
    //    }

    std::string type = cubeMapData.at("filetype").get<std::string>();


    auto texture = std::make_shared<CTextureCubeMap>();

    texture->bind();
    texture->setWrapMode(textures::ETextureWrapMode::eClampToEdge,
                         textures::ETextureWrapMode::eClampToEdge,
                         textures::ETextureWrapMode::eClampToEdge);

    bool hasAlpha;
    for (auto i = 0U; i < cubeMapData["files"].get<int>(); ++i)
    {
        std::string file(fmt::format("{}/{}.{}.png", path.string(), size, i + 1));

        TSurfacePtr surface = loadImage(file.c_str(), hasAlpha);
        const GLenum pixelFormat = hasAlpha ? GL_RGBA : GL_RGB;
        std::cout << file << "(w: " << surface->w << ", h: " << surface->h << ")" << std::endl;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GLint(pixelFormat), surface->w,
                     surface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, surface->pixels);
    }

    texture->genMipMap();
    CTextureCubeMap::unbind();
    return texture;
}
