
#include "CTextureCubeMap.hpp"
#include "core/resources/CResourceLoader.hpp"
#include "core/auxiliary/opengl.hpp"


CTextureCubeMap::CTextureCubeMap()
    : mTextureId(0)
    , mHasAlpha(false)
    , mSize(0, 0)
{
    glGenTextures(1, &mTextureId);
}

CTextureCubeMap::~CTextureCubeMap()
{
    if (0 != mTextureId)
    {
        glDeleteTextures(1, &mTextureId);
    }
}

void CTextureCubeMap::bind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
}

void CTextureCubeMap::unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CTextureCubeMap::genMipMap()
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void CTextureCubeMap::setWrapMode(textures::eTextureWrapMode s, textures::eTextureWrapMode t, textures::eTextureWrapMode r)
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, textures::mapTextureWrapMode(s));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, textures::mapTextureWrapMode(t));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, textures::mapTextureWrapMode(r));
}

