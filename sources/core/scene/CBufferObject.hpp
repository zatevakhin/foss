
#ifndef FOSS_CBUFFEROBJECT_HPP
#define FOSS_CBUFFEROBJECT_HPP

#include "core/auxiliary/opengl.hpp"


#include <boost/noncopyable.hpp>
#include <vector>


enum class EBufferType
{
    eAttributes, // GL_ARRAY_BUFFER
    eIndexes,    // GL_ELEMENT_ARRAY_BUFFER
};

enum class EBufferUsage
{
    eStaticDraw,
    eDynamicDraw,
    eStreamDraw,
};



class CBufferObject : private boost::noncopyable
{
public:
    explicit CBufferObject(EBufferType type, EBufferUsage usage = EBufferUsage::eStaticDraw);
    ~CBufferObject();

    static void unbind(EBufferType type);

    void bind() const;

    void unbind() const;

    void copy(const void *data, size_t size);

    template <class T>
    inline void copy(const std::vector<T> &array)
    {
        const size_t size = sizeof(T) * array.size();
        copy(array.data(), size);
    }

    size_t getSize() const;

private:
    GLuint mId = 0;

    EBufferType mType;
    EBufferUsage mUsage;

};

#endif //FOSS_CBUFFEROBJECT_HPP
