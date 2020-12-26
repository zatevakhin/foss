
#include "app/base/geometry/CTerrainFace.hpp"
#include <glm/matrix.hpp>

#include "app/base/auxiliary/trace.hpp"


CTerrainFace::CTerrainFace(CMesh &mesh, int resolution, glm::vec3 localUp)
    : mMesh(mesh)
    , mResolution(resolution)
    , mLocalUp(localUp)
{
    mAxisA = glm::vec3(localUp.y, localUp.z, localUp.x);
    mAxisB = glm::cross(localUp, mAxisA);
}

void CTerrainFace::buildMesh()
{
    glm::vec3 vertices[mResolution * mResolution];
    int triangles[(mResolution - 1) * (mResolution - 1) * 6];
    int triangleIndex = 0;

    for (int y = 0; y < mResolution; ++y)
    {
        for (int x = 0; x < mResolution; ++x)
        {
            int i = x + y * mResolution;
            glm::vec2 percent = glm::vec2(x, y) / glm::vec2(mResolution - 1);
            glm::vec3 pointOnUnitCube = mLocalUp + (percent.x - .5f) * 2 * mAxisA + (percent.y - .5f) * 2 * mAxisB;
            glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);
            vertices[i] = pointOnUnitSphere;    

            if (x < mResolution - 1 && y < mResolution - 1)
            {

                triangles[triangleIndex] = i;
                triangles[triangleIndex + 1] = i + mResolution + 1;
                triangles[triangleIndex + 2] = i + mResolution;

                triangles[triangleIndex + 3] = i;
                triangles[triangleIndex + 4] = i + 1;
                triangles[triangleIndex + 5] = i + mResolution + 1;
                triangleIndex += 6;
            }
        }
    }
    mMesh.setVertices(vertices, mResolution * mResolution);
    mMesh.setIndexes(triangles, triangleIndex);;
    mMesh.bindGeometry();
}


