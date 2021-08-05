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
        if (const auto t = material->getOcclusionTexture())
        {
            mProgram->uniform("aoMap") = t->bind(GL_TEXTURE2);
        }

        if (const auto t = material->getNormalTexture())
        {
            mProgram->uniform("normalMap") = t->bind(GL_TEXTURE1);
        }

        if (const auto t = material->getBaseColorTexture())
        {
            mProgram->uniform("albedoMap") = t->bind(GL_TEXTURE0);
        }
    }
}


void CModelProgramAdapter::unsetMaterial(TMaterialSharedPtr material)
{
    if (material)
    {
        if (const auto t = material->getBaseColorTexture())
        {
            t->unbind();
        }

        if (const auto t = material->getOcclusionTexture())
        {
            t->unbind();
        }

        if (const auto t = material->getNormalTexture())
        {
            t->unbind();
        }
    }
}
