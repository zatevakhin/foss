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
            mProgram->uniform("aoMap") = t->bind(GL_TEXTURE4);
        }

        if (const auto t = material->getNormalTexture())
        {
            mProgram->uniform("normalMap") = t->bind(GL_TEXTURE3);
        }

        if (const auto t = material->getRoughnessTexture())
        {
            mProgram->uniform("roughnessMap") = t->bind(GL_TEXTURE2);
        }

        if (const auto t = material->getMetallicTexture())
        {
            mProgram->uniform("metallicMap") = t->bind(GL_TEXTURE1);
        }

        if (const auto t = material->getBaseColorTexture())
        {
            mProgram->uniform("baseColorMap") = t->bind(GL_TEXTURE0);
        }
    }
}


void CModelProgramAdapter::unsetMaterial(TMaterialSharedPtr material)
{
    if (material)
    {
        if (const auto t = material->getOcclusionTexture())
        {
            t->unbind(GL_TEXTURE4);
        }

        if (const auto t = material->getNormalTexture())
        {
            t->unbind(GL_TEXTURE3);
        }

        if (const auto t = material->getRoughnessTexture())
        {
            t->unbind(GL_TEXTURE2);
        }

        if (const auto t = material->getMetallicTexture())
        {
            t->unbind(GL_TEXTURE1);
        }

        if (const auto t = material->getBaseColorTexture())
        {
            t->unbind(GL_TEXTURE0);
        }
    }
}
