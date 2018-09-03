
#include "CStaticModel3DRenderer.hpp"
#include "core/shading/IShaderProgram.hpp"
#include "core/shading/CVertexAttribute.hpp"
#include "core/scene/SStaticModel3D.hpp"
#include "core/shading/CUniform.hpp"
#include "core/textures/CTextureCubeMap.hpp"
#include "core/resources/CRegistry.hpp"
#include "core/geometry/auxiliary.hpp"
#include "core/textures/auxiliary.hpp"
#include "core/renderers/auxiliary.hpp"
#include "core/resources/SPhongMaterial.hpp"
#include "core/scene/CCamera.hpp"
#include "core/auxiliary/glm.hpp"
#include "core/auxiliary/trace.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>


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

    bind("vertex", layout.mPosition3D, 3, false);
    bind("normal", layout.mNormal, 3, false);
    bind("textureUV", layout.mTexCoord2D, 2, false);
}
