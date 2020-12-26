
#include "app/renderers/CStaticModel3DRenderer.hpp"
#include "app/shading/IShaderProgram.hpp"
#include "app/shading/CVertexAttribute.hpp"
#include "base/scene/SStaticModel3D.hpp"
#include "app/shading/CUniform.hpp"
#include "base/textures/CTextureCubeMap.hpp"
#include "base/resources/CRegistry.hpp"
#include "base/geometry/auxiliary.hpp"
#include "base/textures/auxiliary.hpp"
#include "app/renderers/auxiliary.hpp"
#include "base/resources/SPhongMaterial.hpp"
#include "base/scene/CCamera.hpp"
#include "base/auxiliary/glm.hpp"
#include "base/auxiliary/trace.hpp"

#include "app/shading/C3DModelProgram.hpp"
#include "base/geometry/CSimpleGeometry.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>


namespace
{

    void attributeAction(const char * attr, size_t offset, unsigned int numComponents, bool needClamp, size_t layoutOffset, size_t stride, IShaderProgram& program)
    {
        CVertexAttribute attrVar = program.attribute(attr);
        if (offset == geometry::SGeometryLayout::UNSET)
        {
            attrVar.disablePointer();
        }
        else
        {
            const auto bytesOffset = size_t(layoutOffset + unsigned(offset));
            const auto bytesStride = size_t(stride);
            attrVar.enablePointer();
            attrVar.setFloatsOffset(bytesOffset, bytesStride, numComponents, needClamp);
        }
    };

}


void CStaticModel3DRenderer::use(IShaderProgram & program)
{
    C3DRendererBase::use(program);

    mProgram->uniform("diffuseMap")  = 0; // GL_TEXTURE0
    mProgram->uniform("specularMap") = 1; // GL_TEXTURE1
    mProgram->uniform("emissiveMap") = 2; // GL_TEXTURE2

    // TODO: Move light to scene config ??
    mProgram->uniform("light0.position") = glm::vec4(100.f, 100.f, 100.f, 0);
    mProgram->uniform("light0.diffuse") = glm::vec4(0.8f,  0.5f,  0.5f, 1);
    mProgram->uniform("light0.specular") = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
}

void CStaticModel3DRenderer::draw(SStaticModel3D & model)
{
    if (!mProgram)
    {
        throw std::runtime_error("Cannot draw 3D model while no program set");
    }

    mProgram->uniform("projection") = mProjection;
    mProgram->uniform("view") = mView;

    model.mGeometry->bind();

    for (CStaticMesh3D & mesh : model.mMeshes)
    {
        applyModelView(mesh.mLocal);
        applyMaterial(model.mMaterials[mesh.mMaterialIndex]);
        bindAttributes(mesh.mLayout);
        renderers::drawRangeElements(mesh.mLayout);
    }
    model.mGeometry->unbind();
}

void CStaticModel3DRenderer::applyModelView(const glm::mat4 & local)
{
    const glm::mat4 worldMatrix = mView * mTransform * local;
    const glm::mat4 normalMatrix = glm::transpose(glm::inverse(worldMatrix));

    mProgram->uniform("normalModelView") = normalMatrix;
    mProgram->uniform("modelView") = worldMatrix;
}

void CStaticModel3DRenderer::applyMaterial(const SPhongMaterial & material) const
{
    mProgram->uniform("material.shininess") = material.mShininess;
    mProgram->uniform("material.diffuse") = material.mDiffuseColor;
    mProgram->uniform("material.specular") = material.mSpecularColor;
    mProgram->uniform("material.emissive") = material.mEmissiveColor;

    textures::tryBindTexture(material.mEmissive, GL_TEXTURE2, textures::ETextureType::eTexture2D);
    textures::tryBindTexture(material.mSpecular, GL_TEXTURE1, textures::ETextureType::eTexture2D);
    textures::tryBindTexture(material.mDiffuse, GL_TEXTURE0, textures::ETextureType::eTexture2D);

    /// Texture GL_TEXTURE0 must be binded at last.
}

void CStaticModel3DRenderer::bindAttributes(const geometry::SGeometryLayout &layout) const
{
    attributeAction("vertex", layout.mPosition3D, 3, false, layout.mBaseVertexOffset, layout.mVertexSize, *mProgram);
    attributeAction("normal", layout.mNormal, 3, false, layout.mBaseVertexOffset, layout.mVertexSize, *mProgram);
    attributeAction("textureUV", layout.mTexCoord2D, 2, false, layout.mBaseVertexOffset, layout.mVertexSize, *mProgram);
}
