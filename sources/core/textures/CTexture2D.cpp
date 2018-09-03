
#include "auxiliary/opengl.hpp"
#include "CTexture2D.hpp"

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


glm::ivec2 CTexture2D::size() const
{
    return mSize;
}


bool CTexture2D::isAlpha() const
{
    return mHasAlpha;
}


void CTexture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTextureId);

}

void CTexture2D::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CTexture2D::setSurface(const SDL_Surface& surface)
{
    mHasAlpha = SDL_ISPIXELFORMAT_ALPHA(surface.format->format);
    mSize = { surface.w, surface.h };

    const GLenum pixelFormat = mHasAlpha ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, GLint(pixelFormat), mSize.x, mSize.y,
                 0, pixelFormat, GL_UNSIGNED_BYTE, surface.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}


void CTexture2D::setWrapMode(textures::eTextureWrapMode s, textures::eTextureWrapMode t)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textures::mapTextureWrapMode(s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textures::mapTextureWrapMode(t));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
