#ifndef FOSS_CORE_RENDERERS_C3DRENDERERBASE_HPP
#define FOSS_CORE_RENDERERS_C3DRENDERERBASE_HPP


#include "app/geometry/SGeometryData.hpp"
#include "app/geometry/CMeshObject.hpp"

#include <glm/mat4x4.hpp>


class IShaderProgram;
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

    virtual void use(IShaderProgram &program);
    virtual void draw(SStaticModel3D &model) = 0;
    virtual void draw(const CMeshObject &mesh) {}

protected:
    bool mIsPicked;
    glm::mat4 mView;
    glm::mat4 mTransform;
    glm::mat4 mProjection;

    IShaderProgram *mProgram = nullptr;
};

#endif //FOSS_CORE_RENDERERS_C3DRENDERERBASE_HPP
