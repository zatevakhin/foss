
#include "CVertexBufferObject.hpp"
#include "app/auxiliary/trace.hpp"

#include <map>


namespace
{

inline GLenum mapType(EBufferType type)
{
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
    static const std::map<EBufferType, GLenum> bufferTypeMap = {
        {EBufferType::eArrayBuffer, GL_ARRAY_BUFFER},
        {EBufferType::eElementArrayBuffer, GL_ELEMENT_ARRAY_BUFFER},
    };

    return bufferTypeMap.at(type);
}

inline GLenum mapType(EBufferUsage type)
{
    static const std::map<EBufferUsage, GLenum> bufferUsageMap = {
        {EBufferUsage::eStaticDraw, GL_STATIC_DRAW},
        {EBufferUsage::eDynamicDraw, GL_DYNAMIC_DRAW},
        {EBufferUsage::eStreamDraw, GL_STREAM_DRAW},
    };

    return bufferUsageMap.at(type);
}

} // namespace


CVertexBufferObject::CVertexBufferObject(EBufferType type, EBufferUsage usage)
    : mType(type)
    , mUsage(usage)
{
    glGenBuffers(1, &mId);
    trc_debug("ctor: CVertexBufferObject(%u)", mId);
}


CVertexBufferObject::~CVertexBufferObject()
{
    trc_debug("dtor: CVertexBufferObject(%u)", mId);
    glDeleteBuffers(1, &mId);
}


void CVertexBufferObject::unbind(EBufferType type)
{
    glBindBuffer(mapType(type), 0);
}


void CVertexBufferObject::bind() const
{
    glBindBuffer(mapType(mType), mId);
}


void CVertexBufferObject::unbind() const
{
    CVertexBufferObject::unbind(mType);
}


void CVertexBufferObject::copy(const void* data, size_t size)
{
    const GLenum bufferType = mapType(mType);
    glBindBuffer(bufferType, mId);
    glBufferData(bufferType, size, data, mapType(mUsage));
}
