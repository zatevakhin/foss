#include "CBoundingBoxModel.hpp"


constexpr float AABB_VERTECIES[] = {
    -0.5, -0.5, -0.5, +0.5, -0.5, -0.5, +0.5, +0.5, -0.5, -0.5, +0.5, -0.5,
    -0.5, -0.5, +0.5, +0.5, -0.5, +0.5, +0.5, +0.5, +0.5, -0.5, +0.5, +0.5,
};

constexpr int AABB_INDECIES[] = {
    0, 4, 1, 5, 2, 6, 3, 7, 0, 1, 4, 5, 2, 3, 6, 7, 0, 3, 4, 7, 1, 2, 5, 6,
};

CBoundingBoxModel::CBoundingBoxModel()
    : mMesh()
{
    mMesh.bind();
    mMesh.setVertices(AABB_VERTECIES, sizeof(AABB_VERTECIES));
    mMesh.setIndices(AABB_INDECIES, sizeof(AABB_INDECIES));
    gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    mMesh.unbind();
}

void CBoundingBoxModel::bind() const
{
    mMesh.bind();
}

void CBoundingBoxModel::unbind() const
{
    mMesh.unbind();
}
