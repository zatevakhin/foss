
#include "math.hpp"


namespace math
{

glm::vec3 scale_from_transform(const glm::mat4& transform)
{
    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(transform[0].x, transform[0].y, transform[0].z));
    scale.y = glm::length(glm::vec3(transform[1].x, transform[1].y, transform[1].z));
    scale.z = glm::length(glm::vec3(transform[2].x, transform[2].y, transform[2].z));
    return scale;
}

} // namespace math
