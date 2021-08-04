
#include "CVertexBufferObject.hpp"
#include "app/auxiliary/trace.hpp"


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


CVertexBufferObject::CVertexBufferObject(GLenum type, GLenum usage)
    : mType(type)
    , mUsage(usage)
{
    glGenBuffers(1, &mId);
    spdlog::debug("ctor: CVertexBufferObject({})", mId);
}


CVertexBufferObject::CVertexBufferObject(EBufferType type, EBufferUsage usage)
    : CVertexBufferObject(mapType(type), mapType(usage))
{
    glGenBuffers(1, &mId);
    spdlog::debug("ctor: CVertexBufferObject({})", mId);
}


CVertexBufferObject::~CVertexBufferObject()
{
    spdlog::debug("dtor: CVertexBufferObject({})", mId);
    glDeleteBuffers(1, &mId);
}


void CVertexBufferObject::bind() const
{
    glBindBuffer(mType, mId);
}


void CVertexBufferObject::unbind() const
{
    glBindBuffer(mType, 0);
}


void CVertexBufferObject::copy(const void* data, size_t size)
{
    glBindBuffer(mType, mId);
    glBufferData(mType, size, data, mUsage);
}
