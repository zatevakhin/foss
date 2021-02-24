#pragma once

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#include <tuple>
#include <vector>

namespace gl
{

using TTexParametriList = std::vector<std::tuple<GLenum, GLenum, GLint>>;

inline void vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized,
                                  GLsizei stride, const void* pointer)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

inline void viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    glViewport(x, y, width, height);
}

inline void disable(GLenum cap)
{
    glDisable(cap);
}

inline void enable(GLenum cap)
{
    glEnable(cap);
}

inline void clear_color(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    glClearColor(red, green, blue, alpha);
}

inline void clear(GLbitfield mask)
{
    glClear(mask);
}


inline void tex_parameteri(const TTexParametriList& paramList)
{
    for (const auto& e : paramList)
    {
        auto [target, name, param] = e;
        glTexParameteri(target, name, param);
    }
}


} // namespace gl
