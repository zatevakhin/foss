
#include "app/renderers/CCubeMapRenderer.hpp"
#include "base/scene/SStaticModel3D.hpp"
#include "base/shading/CVertexAttribute.hpp"
#include "base/shading/CUniform.hpp"
#include "base/textures/CTextureCubeMap.hpp"
#include "base/resources/CRegistry.hpp"
#include "base/shading/IShaderProgram.hpp"
#include "app/renderers/auxiliary.hpp"

#include <stdexcept>


void CCubeMapRenderer::draw(SStaticModel3D & model)
{
    if (!mProgram)
    {
        throw std::runtime_error("Cannot draw 3D model while no program set");
    }

    // TODO: do something with this!!
    auto cubeMap = CRegistry::get<CTextureSharedPtr>("texture/skybox");

    mProgram->uniform("projection") = mProjection;
    mProgram->uniform("view") = mView;

    model.mGeometry->bind();
    // TODO: and this!!
    cubeMap->bind();

    for (CStaticMesh3D & mesh : model.mMeshes)
    {
        bindAttributes(mesh.mLayout);
        renderers::drawRangeElements(mesh.mLayout);
    }
    model.mGeometry->unbind();
}

void CCubeMapRenderer::bindAttributes(const geometry::SGeometryLayout & layout) const
{
    auto bind = [&](const char * attr, size_t offset, unsigned int numComponents, bool needClamp) {
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
