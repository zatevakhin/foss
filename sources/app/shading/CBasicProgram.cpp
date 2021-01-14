#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/trace.hpp"

#include "CBasicProgram.hpp"
#include "CUniform.hpp"
#include "CVertexAttribute.hpp"


CBasicProgram::CBasicProgram(unsigned int programId, std::vector<unsigned int> shaders)
    : mProgramId(programId)
    , mShaders(shaders)
{
}

CBasicProgram::~CBasicProgram()
{
    trc_debug("CBasicProgram::dtor(%u)", mProgramId);
    deleteProgram();
}

void CBasicProgram::deleteProgram()
{
    for (GLuint shaderId : mShaders)
    {
        glDetachShader(mProgramId, shaderId);
        glDeleteShader(shaderId);
    }

    mShaders.clear();
    glDeleteProgram(mProgramId);
}

void CBasicProgram::use() const
{
    glUseProgram(mProgramId);
}

void CBasicProgram::replace(unsigned int programId, std::vector<unsigned int>& shaderIds)
{
    trc_debug("CBasicProgram::replace(%u -> %u)", mProgramId, programId);
    deleteProgram();

    mProgramId = programId;
    mShaders = shaderIds;
};

unsigned int CBasicProgram::id() const
{
    return mProgramId;
}

CUniform CBasicProgram::uniform(const char* name)
{
    return CUniform(glGetUniformLocation(mProgramId, name));
}

CVertexAttribute CBasicProgram::attribute(const char* name)
{
    return CVertexAttribute(glGetAttribLocation(mProgramId, name));
}

unsigned int CBasicProgram::attributeId(const char* name)
{
    return glGetAttribLocation(mProgramId, name);
}
