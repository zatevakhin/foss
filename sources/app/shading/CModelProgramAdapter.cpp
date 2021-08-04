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


void CModelProgramAdapter::setMaterial(TMaterialSharedPtr material)
{
    if (material)
    {
        if (const auto occlusionTexture = material->getOcclusionTexture())
        {
            occlusionTexture->bind(GL_TEXTURE2);
            mProgram->uniform("aoMap") = 2;
        }

        if (const auto normalTexture = material->getNormalTexture())
        {
            normalTexture->bind(GL_TEXTURE1);
            mProgram->uniform("normalMap") = 1;
        }

        if (const auto baseColorTexture = material->getBaseColorTexture())
        {
            baseColorTexture->bind(GL_TEXTURE0);
            mProgram->uniform("albedoMap") = 0;
        }
    }
}


void CModelProgramAdapter::unsetMaterial(TMaterialSharedPtr material)
{
    if (material)
    {
        if (const auto baseColorTexture = material->getBaseColorTexture())
        {
            baseColorTexture->unbind();
        }

        if (const auto occlusionTexture = material->getOcclusionTexture())
        {
            occlusionTexture->unbind();
        }

        if (const auto normalTexture = material->getNormalTexture())
        {
            normalTexture->unbind();
        }
    }
}
