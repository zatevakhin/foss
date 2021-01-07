
#include "app/renderers/CMeshRenderer.hpp"
#include "app/scene/SStaticModel3D.hpp"
#include "app/shading/CUniform.hpp"
#include "app/auxiliary/trace.hpp"


void CMeshRenderer::draw(SStaticModel3D & model)
{
}

void CMeshRenderer::draw(const CMeshObject &mesh)
{
    mProgram->uniform("projection") = mProjection;
    mProgram->uniform("view") = mView * (mTransform);

    for (int i = 0; i < mesh.count(); ++i)
    {
        mProgram->uniform("background") = glm::vec4(1.f, 1.f, 0.f, 1.f);

        auto & m = mesh.get(i);

        m.bind();
        glDrawElements(GL_TRIANGLES, m.getIndexesCount(), GL_UNSIGNED_INT, 0);
        m.unbind();
    }
}
