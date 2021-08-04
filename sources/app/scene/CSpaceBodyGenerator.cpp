#include "CSpaceBodyGenerator.hpp"

#include "CTerrainFace.hpp"

#include "app/auxiliary/trace.hpp"
#include "app/scene/Mesh.hpp"
#include "app/scene/Model.hpp"


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
    auto vao = std::make_shared<CVertexArrayObject>();
    vao->bind();

    for (auto& direction : CUBE_FACES_DIRECTIONS)
    {

        CTerrainFace face(mSettings.mResolution, direction);

        TVec3List positions;
        TVec3List normals;
        TIndiceList indices;
        TVboList vbos;

        face.buildMesh(shapeFilter, positions, normals, indices);

        {
            auto vbo = vbos.emplace_back(
                new CVertexBufferObject(EBufferType::eArrayBuffer, EBufferUsage::eStaticDraw));
            vbo->copy(positions);
        }

        {
            auto vbo = vbos.emplace_back(
                new CVertexBufferObject(EBufferType::eArrayBuffer, EBufferUsage::eStaticDraw));
            vbo->copy(normals);
        }

        {
            auto vbo = vbos.emplace_back(new CVertexBufferObject(EBufferType::eElementArrayBuffer,
                                                                 EBufferUsage::eStaticDraw));
            vbo->copy(indices);
        }

        gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
        auto normal_offset = reinterpret_cast<void*>(sizeof(glm::vec3));
        gl::vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), normal_offset);
        auto tex_coord_offset = reinterpret_cast<void*>(sizeof(glm::vec3) * 2);
        gl::vertex_attrib_pointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), tex_coord_offset);

        TBoundingBoxSharedPtr bbox =
            std::for_each(positions.begin(), positions.end(), BoundingBoxBuilder()).get();

        mMeshes.emplace_back(
            new Mesh(vbos, bbox, GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, -1));
    }

    mProceduralModel.reset(new Model(vao, mMeshes, mMaterials));

    vao->unbind();

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
