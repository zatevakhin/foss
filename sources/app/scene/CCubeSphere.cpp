
#include "CCubeSphere.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

#include "CStaticModel.hpp"
#include "Mesh.hpp"
#include "app/auxiliary/trace.hpp"
#include "app/math/noise.hpp"

namespace
{

constexpr int CUBE_FACES_COUNT = 6;

class CTerrainFace
{

public:
    CTerrainFace(int resolution, glm::vec3 localUp)
        : mResolution(resolution)
        , mLocalUp(localUp)
        , mNoise(1)
    {
        mAxisA = glm::vec3(localUp.y, localUp.z, localUp.x);
        mAxisB = glm::cross(localUp, mAxisA);
    }

    void buildMesh(TVerticeList& vertices, TIndiceList& indices)
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

                auto elevation = (mNoise.evaluate(pointOnUnitSphere) + 1.f) * 0.5f;
                vertices[i].position = pointOnUnitSphere * (1 + elevation);

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

private:
    const int mResolution;
    const glm::vec3 mLocalUp;
    glm::vec3 mAxisA;
    glm::vec3 mAxisB;
    Noise mNoise;
};


} // namespace


CCubeSphere::CCubeSphere(int resolution)
    : mResolution(resolution)
{
}

TModelPtr CCubeSphere::getModel() const
{
    std::array<glm::vec3, CUBE_FACES_COUNT> directions = {
        glm::vec3(0, 1, 0),  glm::vec3(0, -1, 0), glm::vec3(1, 0, 0),
        glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1),  glm::vec3(0, 0, -1),
    };

    TMeshesList meshes;

    auto material = TPhongMaterialPtr(new SMaterialPhong());
    material->mDiffuseColor = glm::vec4(0.f, 0.f, 0.f, 1.f);
    material->mEmissiveColor = glm::vec4(1.f, 1.f, 0.f, 1.f);
    material->mSpecularColor = glm::vec4(0.f, 0.f, 0.f, 1.f);

    std::for_each(directions.begin(), directions.end(),
                  [this, &meshes, &material](auto& direction) {
                      CTerrainFace face(mResolution, direction);

                      TVerticeList vertices;
                      TIndiceList indices;
                      face.buildMesh(vertices, indices);

                      meshes.emplace_back(new Mesh(vertices, indices, material));
                  });

    return std::make_shared<CStaticModel>(meshes);
}
