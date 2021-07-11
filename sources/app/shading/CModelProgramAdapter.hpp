#pragma once

#include "IProgramAdapter.hpp"
#include <glm/matrix.hpp>


class CModelProgramAdapter : public IProgramAdapter
{
public:
    CModelProgramAdapter(TProgramSharedPtr& program);
    ~CModelProgramAdapter() = default;

    void setProjection(glm::mat4 projection) override;
    void setModelAndView(glm::mat4 model, glm::mat4 view) override;
    void setMaterial(TPhongMaterialPtr material) override;
    void setMaterial(TPbrMaterialPtr material) override;

private:
    TProgramSharedPtr mProgram;
};
