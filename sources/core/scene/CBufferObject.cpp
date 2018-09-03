
#include "CBufferObject.hpp"

#include <map>


namespace
{

inline GLenum mapType(eBufferType type)
{
    static const std::map<eBufferType, GLenum> bufferTypeMap = {
        { eBufferType::Attributes, GL_ARRAY_BUFFER },
        { eBufferType::Indexes,   GL_ELEMENT_ARRAY_BUFFER },
    };

    return bufferTypeMap.at(type);
}

inline GLenum mapType(eBufferUsage type)
{
    static const std::map<eBufferUsage, GLenum> bufferUsageMap = {
        { eBufferUsage::StaticDraw,  GL_STATIC_DRAW },
        { eBufferUsage::DynamicDraw, GL_DYNAMIC_DRAW },
        { eBufferUsage::StreamDraw,  GL_STREAM_DRAW },
    };

    return bufferUsageMap.at(type);
}

}


CBufferObject::CBufferObject(eBufferType type, eBufferUsage usage)
    : mType(type)
    , mUsage(usage)
{
    glGenBuffers(1, &mId);
}


CBufferObject::~CBufferObject()
{
    glDeleteBuffers(1, &mId);
}


void CBufferObject::unbind(eBufferType type)
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
