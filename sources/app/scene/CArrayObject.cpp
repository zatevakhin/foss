
#include "CArrayObject.hpp"


CArrayObject::CArrayObject()
{
    glGenVertexArrays(1, &mId);
}

CArrayObject::~CArrayObject()
{
    glDeleteVertexArrays(1, &mId);
}

void CArrayObject::bind() const
{
    glBindVertexArray(mId);
}

void CArrayObject::unbind() const
{
    glBindVertexArray(0U);
}
