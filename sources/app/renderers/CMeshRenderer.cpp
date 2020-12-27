
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



CMeshRenderer::CMeshRenderer()
    // : mSphere(3)
{
    // mSphere.buildMeshes();
    // mSphere.bindGeometry();
}

void CMeshRenderer::draw(SStaticModel3D & model)
{
}

void CMeshRenderer::draw(const CMeshObject &mesh)
{

    mProgram->uniform("projection") = mProjection;
    mProgram->uniform("view") = mView * (mTransform);

    mProgram->uniform("background") = glm::vec4(1.f, 1.f, 0.f, 1.f);
    GLuint vertexAttrib = mProgram->attributeId("vertex");

    for (int i = 0; i < mesh.count(); ++i)
    {   
        auto & m = mesh.get(i);
        m.bind();
        glEnableVertexAttribArray(vertexAttrib);
        glVertexAttribPointer(
            vertexAttrib, // attribute
            3,            // number of elements per vertex, here (x,y,z,w)
            GL_FLOAT,     // the type of each element
            GL_FALSE,     // take our values as-is
            0,            // no extra data between each position
            0             // offset of first element
        );

        const auto indexesCount = m.getIndexesCount();
        glDrawElements(GL_TRIANGLE_STRIP, indexesCount, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(vertexAttrib);
        m.unbind();
    }
}
