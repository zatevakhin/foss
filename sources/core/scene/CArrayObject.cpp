
#include "CArrayObject.hpp"


CArrayObject::CArrayObject()
{
    glGenVertexArrays(1, &mId);
}

CArrayObject::~CArrayObject()
{
    glDeleteVertexArrays(1, &mId);
}

void CArrayObject::bind()
{
    glBindVertexArray(mId);
}

void CArrayObject::unbind()
{
    glBindVertexArray(0U);
}
 