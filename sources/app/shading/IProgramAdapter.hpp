#pragma once

#include "IShaderProgram.hpp"
#include "app/scene/CPbrMaterial.hpp"
#include "app/scene/CPhongMaterial.hpp"

#include <glm/matrix.hpp>


class IProgramAdapter
{
public:
    virtual void setMaterial(TPhongMaterialPtr material) = 0;
    virtual void setMaterial(TPbrMaterialPtr material) = 0;
    virtual void setProjection(glm::mat4 projection) = 0;
    virtual void setModelAndView(glm::mat4 model, glm::mat4 view) = 0;

    virtual ~IProgramAdapter(){};
};

using TProgramAdapterPtr = std::shared_ptr<IProgramAdapter>;
