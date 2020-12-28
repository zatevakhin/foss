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

    static std::string getFileAsString(const fs::path& path);
    static fs::path getAbsolute(const fs::path& path);

    CTextureSharedPtr getTexture(const fs::path& path);
    CTextureSharedPtr getCubeMap(const fs::path& path, unsigned int size);
    TSurfacePtr loadImage(const fs::path& path, bool & hasAlpha);
    CTextureSharedPtr loadTexture(const fs::path& path);

// TODO: --v
//private:
//    CTextureCache mTextureCache;

private:
    CTextureSharedPtr loadCubeMap(const fs::path& path, unsigned int size);

};
