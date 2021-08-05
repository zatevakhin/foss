
#include "app/scene/CBoundingBoxModel.hpp"
// #include "Mesh.hpp"
#include "app/resources/Material.hpp"
#include "app/scene/Mesh.hpp"
#include "app/scene/Model.hpp"

#include "app/auxiliary/glm.hpp"

#include <algorithm>
#include <array>

constexpr std::array AABB_VERTECIES = {
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec3(+0.5f, +0.5f, -0.5f),
    glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec3(+0.5f, -0.5f, +0.5f),
    glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec3(-0.5f, +0.5f, +0.5f),
};

constexpr std::array AABB_INDECIES = {
    0, 4, 1, 5, 2, 6, 3, 7, 0, 1, 4, 5, 2, 3, 6, 7, 0, 3, 4, 7, 1, 2, 5, 6,
};


CBoundingBoxModel::CBoundingBoxModel()
    : mModel(nullptr)
{
    setupModel();
}

void CBoundingBoxModel::setupModel()
{
    auto vao = std::make_shared<CVertexArrayObject>();
    vao->bind();

    TMeshesList meshes;
    TMaterialsList materials;
    TVboList vbos;

    {
        auto vbo = vbos.emplace_back(
            new CVertexBufferObject(EBufferType::eArrayBuffer, EBufferUsage::eStaticDraw));
        vbo->copy(AABB_VERTECIES);
    }

    {
        auto vbo = vbos.emplace_back(
            new CVertexBufferObject(EBufferType::eElementArrayBuffer, EBufferUsage::eStaticDraw));
        vbo->copy(AABB_INDECIES);
    }

    const auto indeciseCount = AABB_INDECIES.size();

    gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    vao->unbind();

    meshes.emplace_back(new Mesh(vbos, nullptr, GL_LINES, indeciseCount, GL_UNSIGNED_INT, 0, -1));

    materials.emplace_back(new Material());
    mModel.reset(new Model(vao, meshes, materials));
}

TModelPtr CBoundingBoxModel::getModel() const
{
    return mModel;
}