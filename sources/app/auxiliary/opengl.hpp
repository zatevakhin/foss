#pragma once

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>


namespace gl
{

    inline void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }

    inline void Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        glViewport(x, y, width, height);
    }

    inline void Disable(GLenum cap)
    {
        glDisable(cap);
    }

    inline void Enable(GLenum cap)
    {
        glEnable(cap);
    }

    inline void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        glClearColor(red, green, blue, alpha);
    }

    inline void Clear(GLbitfield mask)
    {
        glClear(mask);
    }


} // namespace gl
