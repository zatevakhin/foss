#pragma once

#include "app/auxiliary/opengl.hpp"


class CVertexArrayObject
{
public:
    CVertexArrayObject();
    ~CVertexArrayObject();

    CVertexArrayObject(const CVertexArrayObject&) = delete;

    void bind() const;
    void unbind() const;

    GLuint getId() const;

private:
    GLuint mId{0};
};
