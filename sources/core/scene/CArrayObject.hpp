
#ifndef FOSS_CARRAYOBJECT_HPP
#define FOSS_CARRAYOBJECT_HPP

#include "core/auxiliary/opengl.hpp"

class CArrayObject
{
public:
    CArrayObject();
    ~CArrayObject();

    void bind();
    void unbind();

private:
    GLuint mId;
};

#endif //FOSS_CARRAYOBJECT_HPP
