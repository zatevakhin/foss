
#include "CFrameBufferObject.hpp"
#include <iostream>


CFrameBufferObject::CFrameBufferObject()
{
    glGenFramebuffers(1, &mId);
}

CFrameBufferObject::~CFrameBufferObject()
{
    glDeleteFramebuffers(1, &mId);
}

void CFrameBufferObject::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mId);
}

void CFrameBufferObject::attachTexture(unsigned int textureId, unsigned int attachment) const
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureId, 0);
}

bool CFrameBufferObject::isComplete() const
{
    const auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != result)
    {
        std::cout << "Buffer is incomplete = " << result << std::endl;
        return false;
    }

    return true;
}

void CFrameBufferObject::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0U);
}
