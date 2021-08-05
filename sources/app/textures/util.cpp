#include "app/textures/util.hpp"
#include "app/auxiliary/opengl.hpp"
#include <cassert>


int mapToUniformId(int tex)
{
    static GLuint first = GL_TEXTURE0;
    assert(tex <= GL_TEXTURE31);
    return tex - first;
}
