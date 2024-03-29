#pragma once

#include "app/auxiliary/opengl.hpp"
#include <array>
#include <map>
#include <memory>
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
    explicit CVertexBufferObject(GLenum type, GLenum usage);
    explicit CVertexBufferObject(EBufferType type, EBufferUsage usage = EBufferUsage::eStaticDraw);
    ~CVertexBufferObject();

    CVertexBufferObject(const CVertexBufferObject&) = delete;

    GLuint id() const
    {
        return mId;
    }

    void bind() const;

    void unbind() const;

    /**
     * @brief Copying the data to video memory.
     * @param data - Any type of data which can be writen into buffer.
     * @param size - Data size in bytes.
     */
    void copy(const void* data, size_t size);

    template <class T>
    inline void copy(const std::vector<T>& array)
    {
        const size_t size = sizeof(T) * array.size();
        copy(array.data(), size);
    }

    template <class T, size_t N>
    inline void copy(const std::array<T, N>& array)
    {
        const size_t size = sizeof(T) * N;
        copy(array.data(), size);
    }

private:
    GLuint mId{0};

    GLenum mType;
    GLenum mUsage;
};

using TVboSharedPtr = std::shared_ptr<CVertexBufferObject>;
using TVboList = std::vector<TVboSharedPtr>;
