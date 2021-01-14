
#include "app/geometry/CCubeSphere.hpp"

constexpr int CUBE_FACES_COUNT = 6;

CCubeSphere::CCubeSphere(int resolution)
    : CMeshObject()
    , mResolution(resolution)
{
    std::array<glm::vec3, CUBE_FACES_COUNT> directions = {
        glm::vec3(0, 1, 0),  glm::vec3(0, -1, 0), glm::vec3(1, 0, 0),
        glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1),  glm::vec3(0, 0, -1),
    };

    for (int i = 0; i < CUBE_FACES_COUNT; ++i)
    {
        mCubeFaces.emplace_back(create(), mResolution, directions[i]);
        mCubeFaces.back().buildMesh();
    }
}

void CCubeSphere::bind(size_t i) const
{
    get(i).bind();
}

void CCubeSphere::unbind(size_t i) const
{
    get(i).unbind();
}
