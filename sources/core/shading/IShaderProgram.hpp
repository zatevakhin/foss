
#ifndef FOSS_CORE_GRAPHICS_ISHADERPROGRAM_HPP
#define FOSS_CORE_GRAPHICS_ISHADERPROGRAM_HPP

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
};


#endif //FOSS_CORE_GRAPHICS_ISHADERPROGRAM_HPP
