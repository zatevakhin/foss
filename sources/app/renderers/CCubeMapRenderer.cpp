
#include "CCubeMapRenderer.hpp"
#include "core/scene/SStaticModel3D.hpp"
#include "core/shading/CVertexAttribute.hpp"
#include "core/shading/CUniform.hpp"
#include "core/textures/CTextureCubeMap.hpp"
#include "core/resources/CRegistry.hpp"
#include "core/shading/IShaderProgram.hpp"
#include "core/renderers/auxiliary.hpp"

#include <stdexcept>


void CCubeMapRenderer::draw(SStaticModel3D & model)
{
    if (!mProgram)
    {
        throw std::runtime_error("Cannot draw 3D model while no program set");
    }

    auto cubeMap = CRegistry::get<CTextureSharedPtr>("texture/skybox");

    mProgram->uniform("projection") = mProjection;
    mProgram->uniform("view") = mView;

    model.mGeometry->bind();
    cubeMap->bind();

    for (CStaticMesh3D & mesh : model.mMeshes)
    {
        // applyModelView(mesh.mLocal);
        bindAttributes(mesh.mLayout);
        renderers::drawRangeElements(mesh.mLayout);
    }
}


// void CCubeMapRenderer::applyModelView(const glm::mat4 & local)
// {
    // const glm::mat4 worldMatrix = mView * mTransform * local;
    // const glm::mat4 normalMatrix = glm::transpose(glm::inverse(worldMatrix));
    // mProgram->uniform("position") = worldMatrix;
    // mProgram->uniform("normal") = normalMatrix;
    // mProgram->uniform("model") = local;
// }

void CCubeMapRenderer::bindAttributes(const geometry::SGeometryLayout & layout) const
{
    auto bind = [&](const char* attr, size_t offset, unsigned numComponents, bool needClamp) {
        CVertexAttribute attrVar = mProgram->attribute(attr);
        if (offset == geometry::SGeometryLayout::UNSET)
        {
            attrVar.disablePointer();
        }
        else
        {
            const auto bytesOffset = size_t(layout.mBaseVertexOffset + unsigned(offset));
            const auto bytesStride = size_t(layout.mVertexSize);
            attrVar.enablePointer();
            attrVar.setFloatsOffset(bytesOffset, bytesStride, numComponents, needClamp);
        }
    };

    bind("position", layout.mPosition3D, 3, false);
    bind("normal", layout.mNormal, 3, false);
}