#pragma once

#include "app/geometry/SGeometryData.hpp"
#include "app/geometry/CMeshObject.hpp"
#include "app/shading/IShaderProgram.hpp"

#include <glm/mat4x4.hpp>


class SStaticModel3D;


namespace geometry
{
    class SGeometryLayout;
}


class C3DRendererBase
{
public:
    void setIsPicked(const bool value);
    void setViewMatrix(const glm::mat4 &value);
    void setTransformMatrix(const glm::mat4 &value);
    void setProjectionMatrix(const glm::mat4 &value);

    virtual void setProgram(CProgramWeakPtr program);
    virtual void use();
    virtual void draw(SStaticModel3D &model) = 0;
    virtual void draw(const CMeshObject &mesh) {}

protected:
    bool mIsPicked;
    glm::mat4 mView;
    glm::mat4 mTransform;
    glm::mat4 mProjection;

    CProgramSharedPtr mProgram;
};
