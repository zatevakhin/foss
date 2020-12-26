
#include "CUniform.hpp"
#include "app/base/auxiliary/glm.hpp"
#include "app/base/auxiliary/opengl.hpp"


CUniform::CUniform(int location)
    : mLocation(location)
{
}


void CUniform::operator=(int value)
{
    if (-1 != mLocation)
    {
        glUniform1i(mLocation, value);
    }
}


void CUniform::operator=(float value)
{
    if (-1 != mLocation)
    {
        glUniform1f(mLocation, value);
    }
}


void CUniform::operator=(const glm::vec2& value)
{
    if (-1 != mLocation)
    {
        glUniform2fv(mLocation, 1, glm::value_ptr(value));
    }
}


void CUniform::operator=(const glm::ivec2& value)
{
    if (-1 != mLocation)
    {
        glUniform2iv(mLocation, 1, glm::value_ptr(value));
    }
}


void CUniform::operator=(const glm::vec3& value)
{
    if (-1 != mLocation)
    {
        glUniform3fv(mLocation, 1, glm::value_ptr(value));
    }
}


void CUniform::operator=(const glm::vec4& value)
{
    if (-1 != mLocation)
    {
        glUniform4fv(mLocation, 1, glm::value_ptr(value));
    }
}


void CUniform::operator=(const glm::mat3& value)
{
    if (-1 != mLocation)
    {
        glUniformMatrix3fv(mLocation, 1, GL_FALSE, glm::value_ptr(value));
    }
}


void CUniform::operator=(const glm::mat4& value)
{
    if (-1 != mLocation)
    {
        glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(value));
    }
}


void CUniform::operator=(const std::vector<glm::mat4>& value)
{
    if ((-1 != mLocation) && !value.empty())
    {
        const auto count = GLsizei(value.size());
        glUniformMatrix4fv(mLocation, count, GL_FALSE, glm::value_ptr(value[0]));
    }
}
