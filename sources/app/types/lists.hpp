#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>


struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

using TVec2List = std::vector<glm::vec2>;
using TVec3List = std::vector<glm::vec3>;
using TVec4List = std::vector<glm::vec4>;

using TVerticesList = std::vector<Vertex>;
using TIndiceList = std::vector<unsigned int>;
