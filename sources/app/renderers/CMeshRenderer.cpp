
#include "app/scene/SStaticModel3D.hpp"
#include "app/shading/CUniform.hpp"
#include "app/textures/CTextureCubeMap.hpp"
#include "app/resources/CRegistry.hpp"
#include "geometry/auxiliary.hpp"
#include "app/resources/SPhongMaterial.hpp"
#include "app/scene/CCamera.hpp"
#include "app/auxiliary/glm.hpp"
#include "app/auxiliary/trace.hpp"

#include "app/shading/CShaderProgram.hpp"
#include "geometry/CSimpleGeometry.hpp"
#include "app/renderers/CMeshRenderer.hpp"

#include "app/geometry/CTerrainFace.hpp"
#include "app/geometry/CCubeSphere.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>


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

        const auto indexesCount = m.getIndexesCount();

        glDrawElements(GL_TRIANGLES, indexesCount, GL_UNSIGNED_INT, 0);

        m.unbind();
    }
}
