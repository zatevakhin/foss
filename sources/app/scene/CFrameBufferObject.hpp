#pragma once

#include "app/auxiliary/opengl.hpp"


class CFrameBufferObject
{
public:
    CFrameBufferObject();
    ~CFrameBufferObject();

    CFrameBufferObject(const CFrameBufferObject&) = delete;

    void bind() const;
    void attachTexture(unsigned int textureId, unsigned int attachment) const;
    void unbind() const;

private:
    GLuint mId;
};
