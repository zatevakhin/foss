#pragma once

#include <memory>
#include <vector>


class CVertexAttribute;
class CShaderProgram;
class CUniform;


class IShaderProgram
{
public:
    virtual ~IShaderProgram() = default;
    virtual void use() const = 0;
    virtual void replace(unsigned int programId, std::vector<unsigned int>& shaderIds) = 0;
    virtual unsigned int id() const = 0;

    virtual CUniform uniform(const char* name) = 0;
    virtual CVertexAttribute attribute(const char* name) = 0;
    virtual unsigned int attributeId(const char* name) = 0;
};

using CProgramSharedPtr = std::shared_ptr<IShaderProgram>;
using CProgramWeakPtr = std::weak_ptr<IShaderProgram>;
