#include "CTerrainFace.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>


CTerrainFace::CTerrainFace(int resolution, glm::vec3 localUp)
    : mResolution(resolution)
    , mLocalUp(localUp)
{
    mAxisA = glm::vec3(localUp.y, localUp.z, localUp.x);
    mAxisB = glm::cross(localUp, mAxisA);
}


void CTerrainFace::buildMesh(std::function<glm::vec3(glm::vec3)>& shapeFilter,
                             TVerticeList& vertices, TIndiceList& indices)
{
    vertices.resize(mResolution * mResolution);
    indices.resize((mResolution - 1) * (mResolution - 1) * 6);

    int index = 0;

    for (int y = 0; y < mResolution; ++y)
    {
        for (int x = 0; x < mResolution; ++x)
        {
            int i = x + y * mResolution;
            glm::vec2 percent = glm::vec2(x, y) / glm::vec2(mResolution - 1);
            glm::vec3 pointOnUnitCube =
                mLocalUp + (percent.x - .5f) * 2 * mAxisA + (percent.y - .5f) * 2 * mAxisB;
            glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

            auto point = shapeFilter(pointOnUnitSphere);
            vertices[i].position = point;
            vertices[i].normal = glm::normalize(point);

            if ((x < mResolution - 1) && (y < mResolution - 1))
            {
                int obj_i = i + 1;
                indices[index + 3] = i;
                indices[index + 4] = i + mResolution + 1;
                indices[index + 5] = i + mResolution;

                indices[index] = i;
                indices[index + 1] = i + 1;
                indices[index + 2] = i + mResolution + 1;
                index += 6;
            }
        }
    }
}
