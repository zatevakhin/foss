
#ifndef FOSS_CRESOURCELOADER_HPP
#define FOSS_CRESOURCELOADER_HPP


#include "core/auxiliary/sdl.hpp"
#include "core/textures/ITexture.hpp"

#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

enum class eTextureType
{
    Texture2D,
    TextureCubeMap
};

class CResourceLoader : private boost::noncopyable
{

public:
    CResourceLoader();
    ~CResourceLoader();

    static std::string getFileAsString(const fs::path& path);
    static fs::path getAbsolute(const fs::path& path);

    CTextureSharedPtr getTexture(const fs::path& path);
    CTextureSharedPtr getCubeMap(const fs::path& path, unsigned int size);
    TSurfacePtr loadImage(const fs::path& path, bool & hasAlpha);
    CTextureSharedPtr loadTexture(const fs::path& path);

//private:
//    CTextureCache mTextureCache;

private:
    CTextureSharedPtr loadCubeMap(const fs::path& path, unsigned int size);

};

#endif //FOSS_CRESOURCELOADER_HPP
