#pragma once

#include "app/auxiliary/opengl.hpp"

class CArrayObject
{
public:
    CArrayObject();
    ~CArrayObject();

    void bind() const;
    void unbind() const;

private:
    GLuint mId;
};
