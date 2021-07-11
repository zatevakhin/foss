#include "CAsteroidGenerator.hpp"
#include "CTerrainFace.hpp"


#include "Mesh.hpp"
#include "app/auxiliary/trace.hpp"


CAsteroidGenerator::CAsteroidGenerator(IProceduralSettings& settings)
    : mSettings(static_cast<TAsteroidSettings&>(settings))
{
    mNoise.randomize(mSettings.mSeed);
}

void CAsteroidGenerator::setSettings(IProceduralSettings& settings)
{
    auto t = static_cast<TAsteroidSettings&>(settings);

    if (t.mSeed != mSettings.mSeed)
    {
        mNoise.randomize(mSettings.mSeed);
    }

    mSettings = t;
}

const TAsteroidSettings& CAsteroidGenerator::getSettings() const
{
    return mSettings;
}

void CAsteroidGenerator::generate()
{
    std::function<glm::vec3(glm::vec3)> filter = [this](glm::vec3 point) -> glm::vec3 {
        auto& settings = mSettings.mNoiseSettings;

        auto noiseValue = mNoise.evaluate(point * settings.roughness + settings.center);
        noiseValue = (noiseValue + 1.f) * 0.5f;
        noiseValue = noiseValue * settings.strength;

        return point * glm::vec3(mSettings.mRadius) * (1 + noiseValue);
    };

    auto material = std::make_shared<CPhongMaterial>();
    material->mShininess = 30.f;
    material->mDiffuseColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
    material->mEmissiveColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
    material->mSpecularColor = glm::vec4(1.f, 0.f, 0.f, 1.f);

    std::for_each(CUBE_FACES_DIRECTIONS.begin(), CUBE_FACES_DIRECTIONS.end(),
                  [this, &filter, &material](auto& direction) {
                      CTerrainFace face(mSettings.mResolution, direction);

                      TVerticeList vertices;
                      TIndiceList indices;
                      face.buildMesh(filter, vertices, indices);

                      mMeshes.emplace_back(new Mesh(vertices, indices, material, nullptr));
                  });
    mProceduralModel.reset(new CStaticModel(mMeshes));

    mMeshes.clear();
}

TModelPtr CAsteroidGenerator::get() const
{
    return mProceduralModel;
}
