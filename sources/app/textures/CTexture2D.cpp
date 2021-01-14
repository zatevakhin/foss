
#include "CTexture2D.hpp"
#include "app/auxiliary/opengl.hpp"

#include <map>


CTexture2D::CTexture2D()
    : mTextureId(0)
    , mHasAlpha(false)
    , mSize(0, 0)
{
    glGenTextures(1, &mTextureId);
}


CTexture2D::~CTexture2D()
{
    if (0 != mTextureId)
    {
        glDeleteTextures(1, &mTextureId);
    }
}

unsigned int CTexture2D::id() const
{
    return mTextureId;
}

glm::ivec2 CTexture2D::size() const
{
    return mSize;
}

void CTexture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

void CTexture2D::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CTexture2D::setTexture(uint format, uint internalFormat, uint type, glm::ivec2 size,
                            const void* ptr)
{
    mSize = size;
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, internalFormat, type, ptr);
}

void CTexture2D::setWrapMode(ETextureWrapMode s, ETextureWrapMode t)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mapTextureWrapMode(s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mapTextureWrapMode(t));
}

void CTexture2D::setFilter()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void CTexture2D::generateMipMaps() const
{
    glGenerateMipmap(GL_TEXTURE_2D);
}
