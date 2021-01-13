#pragma once

#include "app/auxiliary/opengl.hpp"
#include <vector>


enum class EBufferType
{
    eArrayBuffer,
    eElementArrayBuffer,
};

enum class EBufferUsage
{
    eStaticDraw,
    eDynamicDraw,
    eStreamDraw,
};


class CVertexBufferObject
{
public:
    explicit CVertexBufferObject(EBufferType type, EBufferUsage usage = EBufferUsage::eStaticDraw);
    ~CVertexBufferObject();

    CVertexBufferObject(const CVertexBufferObject&) = delete;


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

private:
    GLuint mId = 0;

    EBufferType mType;
    EBufferUsage mUsage;

};
