#include "CStaticModel3DRenderer.hpp"
#include "base/shading/IShaderProgram.hpp"
#include "base/shading/CVertexAttribute.hpp"
#include "base/scene/SStaticModel3D.hpp"
#include "base/shading/CUniform.hpp"
#include "base/textures/CTextureCubeMap.hpp"
#include "base/resources/CRegistry.hpp"
#include "base/geometry/auxiliary.hpp"
#include "base/textures/auxiliary.hpp"
#include "base/renderers/auxiliary.hpp"
#include "base/resources/SPhongMaterial.hpp"
#include "base/scene/CCamera.hpp"
#include "base/auxiliary/glm.hpp"
#include "base/auxiliary/trace.hpp"

#include "app/shading/C3DModelProgram.hpp"
#include "base/geometry/CSimpleGeometry.hpp"
#include "CBoundingBox3DRenderer.hpp"

// #include "app/components/CMeshComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>


constexpr GLfloat vertices[] = {
    -0.5, -0.5, -0.5, 1.0,
    +0.5, -0.5, -0.5, 1.0,
    +0.5, +0.5, -0.5, 1.0,
    -0.5, +0.5, -0.5, 1.0,
    -0.5, -0.5, +0.5, 1.0,
    +0.5, -0.5, +0.5, 1.0,
    +0.5, +0.5, +0.5, 1.0,
    -0.5, +0.5, +0.5, 1.0,
};

constexpr GLushort indexes[] = {
    0, 4, 1, 5, 2, 6, 3, 7,
    0, 1, 4, 5, 2, 3, 6, 7,
    0, 3, 4, 7, 1, 2, 5, 6
};

CBoundingBox3DRenderer::CBoundingBox3DRenderer()
    : mVertices(EBufferType::eAttributes)
    , mIndexes(EBufferType::eIndexes)
    , mVao()
{
    mVao.bind();
    mVertices.copy(vertices, sizeof(vertices));
    mIndexes.copy(indexes, sizeof(indexes));
    mVao.unbind();
}

void CBoundingBox3DRenderer::draw(SStaticModel3D & model)
{
    const auto & bounds = model.mGeometry->getBoundingBox().getBounds<glm::vec3>();

    glm::vec3 size = glm::vec3(bounds.mMax.x - bounds.mMin.x, bounds.mMax.y - bounds.mMin.y, bounds.mMax.z - bounds.mMin.z);
    glm::vec3 center = glm::vec3((bounds.mMin.x + bounds.mMax.x) / 2, (bounds.mMin.y + bounds.mMax.y) / 2, (bounds.mMin.z + bounds.mMax.z) / 2);
    glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

    mProgram->uniform("projection") = mProjection;
    mProgram->uniform("view") = mView * (mTransform * transform);

    mProgram->uniform("background") = mIsPicked ? glm::vec4(1.f, 0.f, 0.f, 1.f) : glm::vec4(0.f, 1.f, 0.f, 1.f);
    mVao.bind();

    GLuint vertexAttrib = mProgram->attributeId("vertex");

    mVertices.bind();
    glEnableVertexAttribArray(vertexAttrib);
    glVertexAttribPointer(
        vertexAttrib, // attribute
        4,            // number of elements per vertex, here (x,y,z,w)
        GL_FLOAT,     // the type of each element
        GL_FALSE,     // take our values as-is
        0,            // no extra data between each position
        0             // offset of first element
    );

    mIndexes.bind();
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, 0);
    mIndexes.unbind();

    glDisableVertexAttribArray(vertexAttrib);

    mVertices.unbind();
    mVao.unbind();
}
