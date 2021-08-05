
#include "CTextureCubeMap.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/textures/util.hpp"

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

int CTextureCubeMap::bind(unsigned int texture) const
{
    auto uniformId = mapToUniformId(texture);

    glActiveTexture(texture);
    bind();

    return uniformId;
}

void CTextureCubeMap::bind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
}

void CTextureCubeMap::unbind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
