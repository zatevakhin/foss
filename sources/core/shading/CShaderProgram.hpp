
#ifndef FOSS_CSHADERPROGRAM_HPP
#define FOSS_CSHADERPROGRAM_HPP

#include "core/auxiliary/opengl.hpp"

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


enum class eShaderType
{
    Vertex,
    Fragment,
    Geometry
};


class CShaderProgram :
     public IShaderProgram,
     private boost::noncopyable
{
public:
    CShaderProgram();
    ~CShaderProgram();

    void compile(const std::string& source, eShaderType type);

    void link();

    void use() const override;

    CUniform uniform(const char *name) override;
    CVertexAttribute attribute(const char *name) override;

    std::string validate();

private:
    GLuint mProgramId;
    std::vector<GLuint> mShaders;
};

#endif //FOSS_CSHADERPROGRAM_HPP
