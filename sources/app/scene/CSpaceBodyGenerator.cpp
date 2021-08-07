#include "CSpaceBodyGenerator.hpp"
#include "CTerrainFace.hpp"
#include "CVertexArrayObject.hpp"
#include "CVertexBufferObject.hpp"

#include "app/auxiliary/trace.hpp"
#include "app/resources/CRegistry.hpp"
#include "app/resources/CTextureManager.hpp"
#include "app/scene/Mesh.hpp"
#include "app/scene/Model.hpp"
#include "app/textures/CTexture2D.hpp"

CSpaceBodyGenerator::CSpaceBodyGenerator(IProceduralSettings& settings)
    : mSettings(static_cast<TSpaceBodySettings&>(settings))
    , mMeshes()
{
    mNoise.randomize(mSettings.mSeed);
}

void CSpaceBodyGenerator::setSettings(IProceduralSettings& settings)
{
    auto t = static_cast<TSpaceBodySettings&>(settings);

    if (t.mSeed != mSettings.mSeed)
    {
        mNoise.randomize(mSettings.mSeed);
    }

    mSettings = t;
}

const TSpaceBodySettings& CSpaceBodyGenerator::getSettings() const
{
    return mSettings;
}


void CSpaceBodyGenerator::generate2(std::function<glm::vec3(glm::vec3)> shapeFilter)
{
    auto mat = mMaterials.emplace_back(new Material());
    mat->setBaseColorFactor(glm::vec4(0.77f, 0.78f, 0.78f, 1.f));
    mat->setRoughnessFactor(1.f);
    mat->setMetallicFactor(1.f);

    for (auto& direction : CUBE_FACES_DIRECTIONS)
    {
        auto vao = std::make_shared<CVertexArrayObject>();

        CTerrainFace face(mSettings.mResolution, direction);

        TVec3List positions;
        TVec3List normals;
        TIndiceList indices;
        TVboList vbos;

        face.buildMesh(shapeFilter, positions, normals, indices);

        vao->bind();

        vbos.emplace_back(
                new CVertexBufferObject(EBufferType::eArrayBuffer, EBufferUsage::eStaticDraw))
            ->copy(positions);


        vbos.emplace_back(
                new CVertexBufferObject(EBufferType::eArrayBuffer, EBufferUsage::eStaticDraw))
            ->copy(normals);

        vbos.emplace_back(new CVertexBufferObject(EBufferType::eElementArrayBuffer,
                                                  EBufferUsage::eStaticDraw))
            ->copy(indices);

        gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
        auto normal_offset = reinterpret_cast<void*>(positions.size());
        gl::vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), normal_offset);
        vao->unbind();

        TBoundingBoxSharedPtr bbox =
            std::for_each(positions.begin(), positions.end(), BoundingBoxBuilder()).get();

        MeshInfo meshInfo;

        meshInfo.mode = GL_TRIANGLES;
        meshInfo.count = indices.size();
        meshInfo.type = GL_UNSIGNED_INT;
        meshInfo.materialId = mMaterials.size() - 1;

        mMeshes.emplace_back(new Mesh(vao, vbos, bbox, meshInfo));
    }


    mProceduralModel.reset(new Model(mMeshes, mMaterials));


    mMeshes.clear();
}

void CSpaceBodyGenerator::generate()
{
    std::function<glm::vec3(glm::vec3)> filter = [this](glm::vec3 point) -> glm::vec3 {
        auto& settings = mSettings.mNoiseSettings;

        auto noiseValue = mNoise.evaluate(point * settings.roughness + settings.center);
        noiseValue = (noiseValue + 1.f) * 0.5f;
        noiseValue = noiseValue * settings.strength;

        return point * glm::vec3(mSettings.mRadius) * (1 + noiseValue);
    };

    generate2(filter);
}

TModelPtr CSpaceBodyGenerator::get() const
{
    return mProceduralModel;
}
