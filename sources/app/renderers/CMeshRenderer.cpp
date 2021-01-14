
#include "app/renderers/CMeshRenderer.hpp"
#include "app/auxiliary/trace.hpp"
#include "app/auxiliary/util.hpp"
#include "app/scene/SStaticModel3D.hpp"
#include "app/shading/CUniform.hpp"

void CMeshRenderer::draw(SStaticModel3D& model)
{
}

void CMeshRenderer::draw(const CMeshObject& mesh)
{
    mProgram->uniform("projection") = mProjection;
    mProgram->uniform("view") = mView * mTransform;

    for (int i = 0; i < mesh.count(); ++i)
    {
        mProgram->uniform("background") = glm::vec4(util::color(255, 255, 0), 1.f);
        auto& m = mesh.get(i);

        m.bind();
        glDrawElements(GL_TRIANGLES, m.getIndicesCount(), GL_UNSIGNED_INT, 0);
        m.unbind();
    }
}
