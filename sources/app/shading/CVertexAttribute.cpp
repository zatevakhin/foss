
#include "CVertexAttribute.hpp"

#include "app/auxiliary/opengl.hpp"


CVertexAttribute::CVertexAttribute(int location)
    : mLocation(location)
{
}

bool CVertexAttribute::isValid() const
{
    return (UNDEFINED != mLocation);
}

void CVertexAttribute::enablePointer()
{
    if (UNDEFINED != mLocation)
    {
        glEnableVertexAttribArray(GLuint(mLocation));
    }
}

void CVertexAttribute::disablePointer()
{
    if (UNDEFINED != mLocation)
    {
        glDisableVertexAttribArray(GLuint(mLocation));
    }
}

void CVertexAttribute::setDivisor(unsigned divisor)
{
    if (UNDEFINED != mLocation)
    {
        glVertexAttribDivisor(GLuint(mLocation), divisor);
    }
}

void CVertexAttribute::setVec3Offset(size_t offset, size_t stride, bool needClamp)
{
    if (UNDEFINED != mLocation)
    {
        const GLboolean normalize = needClamp ? GL_TRUE : GL_FALSE;
        glVertexAttribPointer(GLuint(mLocation), 3, GL_FLOAT, normalize, GLsizei(stride),
                              reinterpret_cast<const void*>(offset));
    }
}

void CVertexAttribute::setVec2Offset(size_t offset, size_t stride)
{
    if (UNDEFINED != mLocation)
    {
        const GLboolean normalize = GL_FALSE;
        glVertexAttribPointer(GLuint(mLocation), 2, GL_FLOAT, normalize, GLsizei(stride),
                              reinterpret_cast<const void*>(offset));
    }
}

void CVertexAttribute::setFloatsOffset(size_t offset, size_t stride, unsigned numComponents,
                                       bool needClamp)
{
    if (UNDEFINED != mLocation)
    {
        const GLboolean normalize = needClamp ? GL_TRUE : GL_FALSE;
        glVertexAttribPointer(GLuint(mLocation), GLint(numComponents), GL_FLOAT, normalize,
                              GLsizei(stride), reinterpret_cast<const void*>(offset));
    }
}

void CVertexAttribute::setUint8Offset(size_t offset, size_t stride, unsigned numComponents)
{
    if (UNDEFINED != mLocation)
    {
        glVertexAttribIPointer(GLuint(mLocation), GLint(numComponents), GL_UNSIGNED_BYTE,
                               GLsizei(stride), reinterpret_cast<const void*>(offset));
    }
}
