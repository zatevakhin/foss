
#include "app/geometry/CCubeSphere.hpp"

constexpr int CUBE_FACES_COUNT = 6;


CCubeSphere::CCubeSphere(int resolution)
    : CMeshObject()
    , mResolution(resolution)
{
    std::array<glm::vec3, CUBE_FACES_COUNT> directions = {
        glm::vec3( 0,  1,  0),
        glm::vec3( 0, -1,  0),
        glm::vec3( 1,  0,  0),
        glm::vec3(-1,  0,  0),
        glm::vec3( 0,  0,  1),
        glm::vec3( 0,  0, -1),
    };

    for (int i = 0; i < CUBE_FACES_COUNT; ++i)
    {
        mCubeFaces.emplace_back(create(), mResolution, directions[i]);
    }

    for (int i = 0; i < CUBE_FACES_COUNT; ++i)
    {
        mCubeFaces[i].buildMesh();
    }
}

void CCubeSphere::bindGeometry()
{
    for (int i = 0; i < CUBE_FACES_COUNT; ++i)
    {
        mGeometries[i].copy(get(i).getVertices(), get(i).getIndexes());
    }
}

void CCubeSphere::bind(int i)
{
    mGeometries[i].bind();
}

void CCubeSphere::unbind(int i)
{
    mGeometries[i].unbind();
}