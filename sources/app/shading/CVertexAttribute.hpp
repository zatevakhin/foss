#pragma once

#include <glm/fwd.hpp>
#include <cstdlib>
#include <cstdint>

class CShaderProgram;

class CVertexAttribute
{
public:
    explicit CVertexAttribute(int location);

    bool isValid() const;

    void enablePointer();
    void disablePointer();

    void setDivisor(unsigned divisor);
    void setVec3Offset(size_t offset, size_t stride, bool needClamp);
    void setVec2Offset(size_t offset, size_t stride);
    void setFloatsOffset(size_t offset, size_t stride, unsigned numComponents, bool needClamp);
    void setUint8Offset(size_t offset, size_t stride, unsigned numComponents);

private:
    int mLocation = -1;
};
