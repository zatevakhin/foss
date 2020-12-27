#pragma once

class CVertexAttribute;
class CShaderProgram;
class CUniform;

class IShaderProgram
{
public:
    virtual ~IShaderProgram() = default;
    virtual void use() const = 0;

    virtual CUniform uniform(const char *name) = 0;
    virtual CVertexAttribute attribute(const char *name) = 0;
    virtual unsigned int attributeId(const char *name) = 0;
};
