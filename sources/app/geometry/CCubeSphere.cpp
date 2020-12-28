
#include "app/geometry/CCubeSphere.hpp"


CCubeSphere::CCubeSphere(int resolution)
    : CMeshObject()
    , mResolution(resolution)
{
    std::array<glm::vec3, 6> directions = {
        glm::vec3(0, 1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, -1),
    };

    for (int i = 0; i < 6; ++i)
    {
        mCubeFaces.emplace_back(create(), mResolution, directions[i]);
    }
}

void CCubeSphere::buildMeshes()
{
    for (int i = 0; i < 6; ++i)
    {
        mCubeFaces[i].buildMesh();
    }
}

void CCubeSphere::bindGeometry()
{
    for (int i = 0; i < 6; ++i)
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