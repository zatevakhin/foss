
#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/scene/SStaticModel3D.hpp"
#include "app/shading/CUniform.hpp"


CBoundingBox3DRenderer::CBoundingBox3DRenderer()
    : mBoxModel()
{
}

void CBoundingBox3DRenderer::draw(SStaticModel3D& model)
{
    drawBoundingBox(model.mGeometry->getBoundingBox());
}

void CBoundingBox3DRenderer::draw(const CMeshObject& mesh)
{
    drawBoundingBox(mesh.getBoundingBox());
}

void CBoundingBox3DRenderer::drawBoundingBox(const geometry::CBoundingBox& aabb)
{
    glm::mat4 AABBTransform =
        glm::translate(glm::mat4(1), aabb.getCenter()) * glm::scale(glm::mat4(1), aabb.getSize());

    mProgram->uniform("background") =
        mIsPicked ? glm::vec4(1.f, 0.f, 0.f, 1.f) : glm::vec4(0.f, 1.f, 0.f, 1.f);

    mProgram->uniform("projection") = mProjection;
    // Making AABB oriented as 3D object.
    mProgram->uniform("view") = mView * (mTransform * AABBTransform);

    mBoxModel.bind();
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    mBoxModel.unbind();
}
