
#include "CTexture2D.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/textures/util.hpp"

#include <map>


CTexture2D::CTexture2D()
    : mTextureId(0)
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

int CTexture2D::bind(unsigned int texture) const
{
    auto uniformId = mapToUniformId(texture);
    glActiveTexture(texture);
    bind();

    return uniformId;
}

void CTexture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

void CTexture2D::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
