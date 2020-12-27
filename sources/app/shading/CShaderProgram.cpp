
#include "CShaderProgram.hpp"
#include "CVertexAttribute.hpp"
#include "app/auxiliary/opengl.hpp"

#include <map>


namespace
{

class CShaderRaii
{
public:
    explicit CShaderRaii(EShaderType type)
    {
        mId = glCreateShader(CShaderRaii::mapShaderType(type));
    }

    ~CShaderRaii()
    {
        glDeleteShader(mId);
    }

    inline const GLuint & getId() const
    {
        return mId;
    }

    inline GLuint release()
    {
        const GLuint id = mId;
        mId = 0;
        return id;
    }

    CShaderRaii(const CShaderRaii &) = delete;
    CShaderRaii& operator=(const CShaderRaii &) = delete;

private:
    GLuint mId;

private:

    static GLenum mapShaderType(EShaderType type)
    {
        static const std::map<EShaderType, GLenum> typeMap = {
            {EShaderType::eVertex,   GL_VERTEX_SHADER},
            {EShaderType::eFragment, GL_FRAGMENT_SHADER},
            {EShaderType::eGeometry, GL_GEOMETRY_SHADER},
            // other shader types
        };

        return typeMap.at(type);
    }
};

std::string getInfoLog(GLuint shaderId)
{
    GLsizei infoLogLength = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::string log(size_t(infoLogLength), '\0');
    glGetShaderInfoLog(shaderId, infoLogLength, &infoLogLength, &log[0]);

    // Cut log to real length
    if (size_t(infoLogLength) < log.length())
    {
        log.erase(log.begin() + ptrdiff_t(infoLogLength), log.end());
    }

    return std::move(log);
}

}


CShaderProgram::CShaderProgram()
    : mProgramId(glCreateProgram())
{
}


CShaderProgram::~CShaderProgram()
{
    for (GLuint shaderId : mShaders)
    {
        glDetachShader(mProgramId, shaderId);
        glDeleteShader(shaderId);
    }

    mShaders.clear();
    glDeleteProgram(mProgramId);
}


void CShaderProgram::compile(const std::string& source, EShaderType type)
{
    const GLchar *shaderSource[] = { source.c_str() };
    const GLint sourceLength[] = { GLint(source.size()) };

    CShaderRaii shader(type);
    glShaderSource(shader.getId(), 1, shaderSource, sourceLength);
    glCompileShader(shader.getId());

    GLint compileStatus = 0;
    glGetShaderiv(shader.getId(), GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE)
    {
        const auto log = getInfoLog(shader.getId());
        throw std::runtime_error("Shader compiling failed: " + log);
    }

    mShaders.emplace_back(shader.release());
    glAttachShader(mProgramId, mShaders.back());
}


void CShaderProgram::link()
{
    glLinkProgram(mProgramId);

    GLint linkStatus = 0;
    glGetProgramiv(mProgramId, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE)
    {
        const auto log = getInfoLog(mProgramId);
        throw std::runtime_error("Program linking failed: " + log);
    }
}


void CShaderProgram::use() const
{
    glUseProgram(mProgramId);
}


CUniform CShaderProgram::uniform(const char* name)
{
    return CUniform(glGetUniformLocation(mProgramId, name));
}


std::string CShaderProgram::validate()
{
    glValidateProgram(mProgramId);

    GLint validateStatus = 0;
    glGetProgramiv(mProgramId, GL_VALIDATE_STATUS, &validateStatus);

    std::string log;
    if (validateStatus == GL_FALSE)
    {
        log = getInfoLog(mProgramId);
    }

    return std::move(log);
}

CVertexAttribute CShaderProgram::attribute(const char *name)
{
    return CVertexAttribute(glGetAttribLocation(mProgramId, name));
}

unsigned int CShaderProgram::attributeId(const char *name)
{
    return glGetAttribLocation(mProgramId, name);
}
