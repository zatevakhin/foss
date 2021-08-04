#pragma once

#include "IShaderProgram.hpp"
#include "app/resources/Material.hpp"

#include <glm/matrix.hpp>


class IProgramAdapter
{
public:
    virtual void setMaterial(TMaterialSharedPtr material) = 0;
    virtual void unsetMaterial(TMaterialSharedPtr material) = 0;
    virtual void setProjection(glm::mat4 projection) = 0;
    virtual void setModelAndView(glm::mat4 model, glm::mat4 view) = 0;

    virtual ~IProgramAdapter(){};
};

using TProgramAdapterPtr = std::shared_ptr<IProgramAdapter>;
