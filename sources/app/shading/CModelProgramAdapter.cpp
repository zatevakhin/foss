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

void CModelProgramAdapter::setMaterial(TPbrMaterialPtr material)
{
    if (material->mAlbedoTex)
    {
        glActiveTexture(GL_TEXTURE0);
        material->mAlbedoTex->bind();
        mProgram->uniform("albedoMap") = 0;
    }
}
