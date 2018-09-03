
#ifndef FOSS_CTEXTURECUBEMAP_HPP
#define FOSS_CTEXTURECUBEMAP_HPP

#include "ITexture.hpp"
#include "CTextureCache.hpp"
#include "core/textures/auxiliary.hpp"
#include "core/auxiliary/glm.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>


namespace fs = boost::filesystem;

class CResourceLoader;


class CTextureCubeMap :
    private boost::noncopyable,
    public ITexture
{
public:
    CTextureCubeMap();
    ~CTextureCubeMap() override;

    void bind() const override;
    static void unbind();

    void genMipMap();
    void setWrapMode(textures::eTextureWrapMode s, textures::eTextureWrapMode t, textures::eTextureWrapMode r);

private:
    unsigned int mTextureId;
    glm::ivec2 mSize;
    bool mHasAlpha;
};

#endif //FOSS_CTEXTURECUBEMAP_HPP
