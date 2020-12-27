#pragma once

#include "ITexture.hpp"
#include "CTextureCache.hpp"
#include "auxiliary.hpp"
#include "app/auxiliary/glm.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>


namespace fs = boost::filesystem;

using namespace textures;


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
    void setWrapMode(ETextureWrapMode s, ETextureWrapMode t, ETextureWrapMode r);

private:
    unsigned int mTextureId;
    glm::ivec2 mSize;
    bool mHasAlpha;
};
