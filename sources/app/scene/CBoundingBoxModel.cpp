
#include "app/scene/CBoundingBoxModel.hpp"
#include "app/resources/Material.hpp"
#include "app/scene/CVertexArrayObject.hpp"
#include "app/scene/CVertexBufferObject.hpp"
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

    vbos.emplace_back(new CVertexBufferObject(EBufferType::eArrayBuffer, EBufferUsage::eStaticDraw))
        ->copy(AABB_VERTECIES);

    vbos.emplace_back(
            new CVertexBufferObject(EBufferType::eElementArrayBuffer, EBufferUsage::eStaticDraw))
        ->copy(AABB_INDECIES);


    gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    vao->unbind();

    MeshInfo meshInfo;

    meshInfo.mode = GL_LINES;
    meshInfo.count = AABB_INDECIES.size();
    meshInfo.type = GL_UNSIGNED_INT;

    meshes.emplace_back(new Mesh(vao, vbos, nullptr, meshInfo));
    mModel.reset(new Model(meshes, materials));
}

TModelPtr CBoundingBoxModel::getModel() const
{
    return mModel;
}
