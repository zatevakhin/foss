#pragma once

#include "glm.hpp"


namespace util
{

    inline glm::vec3 color(float r, float g, float b)
    {
        return glm::vec3((1.0 / 255) * r, (1.0 / 255) * g, (1.0 / 255) * b);
    }

} // namespace util


