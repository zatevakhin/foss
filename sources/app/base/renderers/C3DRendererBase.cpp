
#include "C3DRendererBase.hpp"
#include "app/base/shading/IShaderProgram.hpp"


void C3DRendererBase::setIsPicked(const bool value)
{
    mIsPicked = value;
}

void C3DRendererBase::setViewMatrix(const glm::mat4 & value)
{
    mView = value;
}

void C3DRendererBase::setTransformMatrix(const glm::mat4 & value)
{
    mTransform = value;
}

void C3DRendererBase::setProjectionMatrix(const glm::mat4 & value)
{
    mProjection = value;
}

void C3DRendererBase::use(IShaderProgram & program)
{
    mProgram = &program;
    mProgram->use();
}