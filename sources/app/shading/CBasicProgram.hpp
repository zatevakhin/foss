#pragma once

#include "IShaderProgram.hpp"
#include "CVertexAttribute.hpp"
#include "CUniform.hpp"
#include <vector>


class CBasicProgram : public IShaderProgram
{

public:
    CBasicProgram(unsigned int, std::vector<unsigned int>);
    ~CBasicProgram();

    CBasicProgram(const CBasicProgram &) = delete;
    CBasicProgram& operator=(const CBasicProgram &) = delete;

    void use() const override;
    void replace(unsigned int programId, std::vector<unsigned int>& shaderIds) override;
    unsigned int id() const override;

    CUniform uniform(const char* name) override;
    CVertexAttribute attribute(const char* name) override;
    unsigned int attributeId(const char* name) override;

private:
    void deleteProgram();

private:
    unsigned int mProgramId;
    std::vector<unsigned int> mShaders;

};


