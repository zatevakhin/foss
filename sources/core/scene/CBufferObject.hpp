
#ifndef FOSS_CBUFFEROBJECT_HPP
#define FOSS_CBUFFEROBJECT_HPP

#include "core/auxiliary/opengl.hpp"


#include <boost/noncopyable.hpp>
#include <vector>


enum class eBufferType
{
    Attributes, // GL_ARRAY_BUFFER
    Indexes,    // GL_ELEMENT_ARRAY_BUFFER
};

enum class eBufferUsage
{
    StaticDraw,
    DynamicDraw,
    StreamDraw,
};



class CBufferObject : private boost::noncopyable
{
public:
    explicit CBufferObject(eBufferType type, eBufferUsage usage = eBufferUsage::StaticDraw);
    ~CBufferObject();

    static void unbind(eBufferType type);

    void bind() const;

    void unbind() const;

    void copy(const void *data, size_t size);

    template <class T>
    inline void copy(const std::vector<T> &array)
    {
        const size_t size = sizeof(T) * array.size();
        this->copy(array.data(), size);
    }

    size_t getSize() const;

private:
    GLuint mId = 0;

    eBufferType mType;
    eBufferUsage mUsage;

};

#endif //FOSS_CBUFFEROBJECT_HPP
