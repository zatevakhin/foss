
#include "CVertexArrayObject.hpp"


CVertexArrayObject::CVertexArrayObject()
{
    glGenVertexArrays(1, &mId);
}

CVertexArrayObject::~CVertexArrayObject()
{
    glDeleteVertexArrays(1, &mId);
}

void CVertexArrayObject::bind() const
{
    glBindVertexArray(mId);
}

void CVertexArrayObject::unbind() const
{
    glBindVertexArray(0U);
}
