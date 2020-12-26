
#ifndef FOSS_CARRAYOBJECT_HPP
#define FOSS_CARRAYOBJECT_HPP

#include "app/base/auxiliary/opengl.hpp"

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

#endif //FOSS_CARRAYOBJECT_HPP
