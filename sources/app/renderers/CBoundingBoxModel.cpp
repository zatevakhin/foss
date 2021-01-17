#include "CBoundingBoxModel.hpp"


constexpr float AABB_VERTECIES[] = {
    -0.5, -0.5, -0.5, +0.5, -0.5, -0.5, +0.5, +0.5, -0.5, -0.5, +0.5, -0.5,
    -0.5, -0.5, +0.5, +0.5, -0.5, +0.5, +0.5, +0.5, +0.5, -0.5, +0.5, +0.5,
};

constexpr int AABB_INDECIES[] = {
    0, 4, 1, 5, 2, 6, 3, 7, 0, 1, 4, 5, 2, 3, 6, 7, 0, 3, 4, 7, 1, 2, 5, 6,
};

CBoundingBoxModel::CBoundingBoxModel()
    : mVerteciesVbo(EBufferType::eArrayBuffer)
    , mIndeciesVbo(EBufferType::eElementArrayBuffer)
    , mVao()
{
    mVao.bind();
    mVerteciesVbo.copy(AABB_VERTECIES, sizeof(AABB_VERTECIES));
    mIndeciesVbo.copy(AABB_INDECIES, sizeof(AABB_INDECIES));

    gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    mVao.unbind();
}

void CBoundingBoxModel::bind() const
{
    mVao.bind();
}

void CBoundingBoxModel::unbind() const
{
    mVao.unbind();
}
