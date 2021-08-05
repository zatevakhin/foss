
#include "CVertexArrayObject.hpp"
#include "app/auxiliary/trace.hpp"


CVertexArrayObject::CVertexArrayObject()
{
    glGenVertexArrays(1, &mId);
    spdlog::debug("ctor: CVertexArrayObject({})", mId);
}

CVertexArrayObject::~CVertexArrayObject()
{
    spdlog::debug("dtor: CVertexArrayObject({})", mId);
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
