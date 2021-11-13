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


glm::vec3 pointOnCubeToPointOnSphere(const glm::vec3& p)
{
    auto x2 = p.x * p.x;
    auto y2 = p.y * p.y;
    auto z2 = p.z * p.z;

    float x = p.x * glm::sqrt(1 - (y2 + z2) / 2 + (y2 * z2) / 3);
    float y = p.y * glm::sqrt(1 - (z2 + x2) / 2 + (z2 * x2) / 3);
    float z = p.z * glm::sqrt(1 - (x2 + y2) / 2 + (x2 * y2) / 3);

    return glm::vec3(x, y, z);
}


void CTerrainFace::buildMesh(std::function<glm::vec3(glm::vec3)>& shapeFilter, TVec3List& positions, TVec3List& normals,
                             TIndiceList& indices)
{
    positions.resize(mResolution * mResolution);
    normals.resize(mResolution * mResolution);
    indices.resize((mResolution - 1) * (mResolution - 1) * 6);

    int index = 0;

    for (int y = 0; y < mResolution; ++y)
    {
        for (int x = 0; x < mResolution; ++x)
        {
            int i = x + y * mResolution;
            glm::vec2 percent = glm::vec2(x, y) / glm::vec2(mResolution - 1);
            glm::vec3 pointOnUnitCube = mLocalUp + (percent.x - .5f) * 2 * mAxisA + (percent.y - .5f) * 2 * mAxisB;

            glm::vec3 pointOnUnitSphere = pointOnCubeToPointOnSphere(pointOnUnitCube);
            // auto pointOnUnitSphere = glm::normalize(pointOnUnitCube);

            auto p = shapeFilter(pointOnUnitSphere);
            positions[i] = p;
            normals[i] = p;

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

void CTerrainFace::buildMesh(std::function<glm::vec3(glm::vec3)>& shapeFilter, TVerticesList& vertices,
                             TIndiceList& indices)
{
    vertices.resize(mResolution * mResolution);
    indices.resize((mResolution - 1) * (mResolution - 1) * 6);

    int index = 0;

    for (int y = 0; y < mResolution; ++y)
    {
        for (int x = 0; x < mResolution; ++x)
        {
            int i = x + y * mResolution;
            glm::vec2 t = glm::vec2(x, y) / glm::vec2(mResolution - 1.f);

            glm::vec3 pointOnUnitCube = mLocalUp + mAxisA * (2.f * t.x - 1.f) + mAxisB * (2 * t.y - 1.f);

            // glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);
            glm::vec3 pointOnUnitSphere = pointOnCubeToPointOnSphere(pointOnUnitCube);

            auto p = shapeFilter(pointOnUnitSphere);
            vertices[i].position = p;
            vertices[i].normal = p;
            vertices[i].uv = glm::vec2(p.x, p.y);

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
