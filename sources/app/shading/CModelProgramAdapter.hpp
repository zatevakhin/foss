#pragma once

#include "IProgramAdapter.hpp"
#include <glm/matrix.hpp>


class CModelProgramAdapter : public IProgramAdapter
{
public:
    CModelProgramAdapter(TProgramSharedPtr& program);
    ~CModelProgramAdapter() = default;

    void setProjection(glm::mat4 projection) override;
    void setView(glm::mat4 view) override;
    void setMaterial(TPhongMaterialPtr material) override;

private:
    TProgramSharedPtr mProgram;
};
