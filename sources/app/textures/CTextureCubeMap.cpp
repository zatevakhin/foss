
#include "CTextureCubeMap.hpp"
#include "app/auxiliary/opengl.hpp"


CTextureCubeMap::CTextureCubeMap()
    : mTextureId(0)
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

unsigned int CTextureCubeMap::id() const
{
    return mTextureId;
}

void CTextureCubeMap::bind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
}

void CTextureCubeMap::unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
