
#ifndef FOSS_CSHADERPROGRAM_HPP
#define FOSS_CSHADERPROGRAM_HPP

#include "CUniform.hpp"
#include "IShaderProgram.hpp"

#include <boost/noncopyable.hpp>
#include <memory>
#include <iostream>
#include <vector>

class CShaderProgram;
class CVertexAttribute;

using CShaderProgramSharedPtr = std::shared_ptr<CShaderProgram>;
using CShaderProgramWeakPtr = std::weak_ptr<CShaderProgram>;


enum class EShaderType
{
    eVertex,
    eFragment,
    eGeometry
};


class CShaderProgram :
     public IShaderProgram,
     private boost::noncopyable
{
public:
    CShaderProgram();
    ~CShaderProgram();

    void compile(const std::string& source, EShaderType type);

    void link();

    void use() const override;

    CUniform uniform(const char *name) override;
    CVertexAttribute attribute(const char *name) override;

    std::string validate();

private:
    unsigned int mProgramId;
    std::vector<unsigned int> mShaders;
};

#endif //FOSS_CSHADERPROGRAM_HPP
