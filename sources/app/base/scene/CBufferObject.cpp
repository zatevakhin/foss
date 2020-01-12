
#include "CBufferObject.hpp"
#include "app/base/auxiliary/trace.hpp"

#include <map>


namespace
{

inline GLenum mapType(EBufferType type)
{
    static const std::map<EBufferType, GLenum> bufferTypeMap = {
        { EBufferType::eAttributes, GL_ARRAY_BUFFER },
        { EBufferType::eIndexes,   GL_ELEMENT_ARRAY_BUFFER },
    };

    return bufferTypeMap.at(type);
}

inline GLenum mapType(EBufferUsage type)
{
    static const std::map<EBufferUsage, GLenum> bufferUsageMap = {
        { EBufferUsage::eStaticDraw,  GL_STATIC_DRAW },
        { EBufferUsage::eDynamicDraw, GL_DYNAMIC_DRAW },
        { EBufferUsage::eStreamDraw,  GL_STREAM_DRAW },
    };

    return bufferUsageMap.at(type);
}

}


CBufferObject::CBufferObject(EBufferType type, EBufferUsage usage)
    : mType(type)
    , mUsage(usage)
{
    glGenBuffers(1, &mId);
    trc_debug("ctor: CBufferObject(%u)", mId);
}


CBufferObject::~CBufferObject()
{
    trc_debug("dtor: CBufferObject(%u)", mId);
    glDeleteBuffers(1, &mId);
}


void CBufferObject::unbind(EBufferType type)
{
    glBindBuffer(mapType(type), 0);
}


void CBufferObject::bind() const
{
    glBindBuffer(mapType(mType), mId);
}


void CBufferObject::unbind() const
{
    CBufferObject::unbind(mType);
}


void CBufferObject::copy(const void* data, size_t size)
{
    const GLenum bufferType = mapType(mType);
    glBindBuffer(bufferType, mId);
    glBufferData(bufferType, size, data, mapType(mUsage));
}


size_t CBufferObject::getSize() const
{
    GLint result = 0;
    glGetBufferParameteriv(mapType(mType), GL_BUFFER_SIZE, &result);
    return static_cast<size_t>(result);
}
