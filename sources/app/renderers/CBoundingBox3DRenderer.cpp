#include "app/shading/IShaderProgram.hpp"
#include "app/shading/CVertexAttribute.hpp"
#include "app/scene/SStaticModel3D.hpp"
#include "app/shading/CUniform.hpp"
#include "app/textures/CTextureCubeMap.hpp"
#include "app/resources/CRegistry.hpp"
#include "geometry/auxiliary.hpp"
#include "app/textures/auxiliary.hpp"
#include "app/renderers/auxiliary.hpp"
#include "app/resources/SPhongMaterial.hpp"
#include "app/scene/CCamera.hpp"
#include "app/auxiliary/glm.hpp"
#include "app/auxiliary/trace.hpp"

#include "app/shading/C3DModelProgram.hpp"
#include "geometry/CSimpleGeometry.hpp"
#include "app/renderers/CBoundingBox3DRenderer.hpp"
#include "app/renderers/CStaticModel3DRenderer.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>


CBoundingBox3DRenderer::CBoundingBox3DRenderer()
    : mBoxModel()
{
}

void CBoundingBox3DRenderer::draw(SStaticModel3D & model)
{
    const auto & aabb = model.mGeometry->getBoundingBox();
    glm::mat4 AABBTransform = glm::translate(glm::mat4(1), aabb.getCenter()) * glm::scale(glm::mat4(1), aabb.getSize());

    mProgram->uniform("background") = mIsPicked ? glm::vec4(1.f, 0.f, 0.f, 1.f) : glm::vec4(0.f, 1.f, 0.f, 1.f);

    mProgram->uniform("projection") = mProjection;
    mProgram->uniform("view") = mView * (mTransform * AABBTransform); // Making AABB oriented as 3D object.

    mBoxModel.bind();
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    mBoxModel.unbind();
}
