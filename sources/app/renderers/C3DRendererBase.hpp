#pragma once

#include "app/geometry/CMeshObject.hpp"
#include "app/scene/IModel.hpp"
#include "app/shading/IShaderProgram.hpp"
#include <glm/mat4x4.hpp>

class C3DRendererBase
{
public:
    void setIsPicked(const bool value);
    void setViewMatrix(const glm::mat4& value);
    void setTransformMatrix(const glm::mat4& value);
    void setProjectionMatrix(const glm::mat4& value);

    virtual void setProgram(TProgramWeakPtr program);
    virtual void use();
    virtual void draw(const CMeshObject& mesh)
    {
    }

    virtual void draw(const TModelPtr model)
    {
    }

protected:
    bool mIsPicked;
    glm::mat4 mView;
    glm::mat4 mTransform;
    glm::mat4 mProjection;

    TProgramSharedPtr mProgram;
};
