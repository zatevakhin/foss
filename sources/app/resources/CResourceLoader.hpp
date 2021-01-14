#pragma once

#include "app/auxiliary/sdl.hpp"
#include "app/textures/ITexture.hpp"

#include <boost/filesystem.hpp>


namespace fs = boost::filesystem;


class CResourceLoader
{

public:
    CResourceLoader();
    ~CResourceLoader();

    CResourceLoader(const CResourceLoader&) = delete;
    CResourceLoader& operator=(const CResourceLoader&) = delete;

    static std::string getFileAsString(const fs::path& path);
    static fs::path getAbsolute(const fs::path& path);

    static CTextureSharedPtr getTexture(const fs::path& path);
    CTextureSharedPtr getCubeMap(const fs::path& path, unsigned int size);
    static TSurfacePtr loadImage(const fs::path& path, bool& hasAlpha);
    static CTextureSharedPtr loadTexture(const fs::path& path);

private:
    CTextureSharedPtr loadCubeMap(const fs::path& path, unsigned int size);
};
