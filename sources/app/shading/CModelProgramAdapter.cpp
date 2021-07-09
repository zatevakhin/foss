#include "CModelProgramAdapter.hpp"
#include "CUniform.hpp"

#include "app/auxiliary/opengl.hpp"


CModelProgramAdapter::CModelProgramAdapter(TProgramSharedPtr& program)
    : mProgram(program)
{
}

void CModelProgramAdapter::setProjection(glm::mat4 projection)
{
    mProgram->uniform("projection") = projection;
}

void CModelProgramAdapter::setModelAndView(glm::mat4 model, glm::mat4 view)
{
    mProgram->uniform("model") = model;
    mProgram->uniform("view") = view;
}

void CModelProgramAdapter::setMaterial(TPhongMaterialPtr material)
{
    mProgram->uniform("material.shininess") = material->mShininess;
    mProgram->uniform("material.specular") = material->mSpecular;
    mProgram->uniform("material.colorDiffuse") = material->mDiffuseColor;
    mProgram->uniform("material.colorSpecular") = material->mSpecularColor;
    mProgram->uniform("material.colorEmissive") = material->mEmissiveColor;

    if (material->mEmissiveTexture)
    {
        glActiveTexture(GL_TEXTURE2);
        material->mEmissiveTexture->bind();
        mProgram->uniform("material.textureEmissive") = 2;
    }

    if (material->mSpecularTexture)
    {
        glActiveTexture(GL_TEXTURE1);
        material->mSpecularTexture->bind();
        mProgram->uniform("material.textureSpecular") = 1;
    }

    if (material->mDiffuseTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        material->mDiffuseTexture->bind();
        mProgram->uniform("material.textureDiffuse") = 0;
    }
}
