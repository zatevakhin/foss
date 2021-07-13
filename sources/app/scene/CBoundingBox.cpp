
#include "CBoundingBox.hpp"
#include "CStaticModel.hpp"
#include "Mesh.hpp"

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


CBoundingBox::CBoundingBox()
    : mModel(nullptr)
{
    setupModel();
}

void CBoundingBox::setupModel()
{
    TMeshesList meshes;
    TMaterialList materials;
    TMaterialToMeshMap mat2mesh;

    TVerticeList vertices;
    TIndiceList indices;

    std::transform(AABB_VERTECIES.begin(), AABB_VERTECIES.end(), std::back_inserter(vertices),
                   [](glm::vec3 p) -> Vertex { return Vertex(p); });

    std::move(AABB_INDECIES.begin(), AABB_INDECIES.end(), std::back_inserter(indices));


    auto mesh = meshes.emplace_back(new Mesh(vertices, indices, GL_LINES));
    mat2mesh.emplace(mesh, nullptr);

    mModel.reset(new CStaticModel(meshes, materials, mat2mesh));
}

TModelPtr CBoundingBox::getModel() const
{
    return mModel;
}
