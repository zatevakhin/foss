
#include "app/geometry/CMesh.hpp"
#include <algorithm>


CMesh::CMesh()
    : mVerticesVbo(EBufferType::eArrayBuffer)
    , mIndicesVbo(EBufferType::eElementArrayBuffer)
    , mNoIndices(false)
    , mVertices()
    , mIndices()
    , mBBox()
    , mVao()
{
}

size_t CMesh::getVerticesCount() const
{
    return mVertices.size();
}

size_t CMesh::getIndicesCount() const
{
    return mIndices.size();
}

const std::vector<float> &CMesh::getVertices() const
{
    return mVertices;
}

const geometry::CBoundingBox &CMesh::getBoundingBox() const
{
    return mBBox;
}

const std::vector<int> &CMesh::getIndices() const
{
    return mIndices;
}

void CMesh::bind() const
{
    mVao.bind();
}

void CMesh::unbind() const
{
    mVao.unbind();
}

void CMesh::clear()
{
    mVertices.clear();
    mIndices.clear();
}

void CMesh::setVertices(const float *vertices, size_t size, bool noIndices)
{
    mNoIndices = noIndices;

    mVertices.reserve(size);
    std::move(vertices, vertices + size, std::back_inserter(mVertices));
    mVertices.shrink_to_fit();

    mVerticesVbo.copy(mVertices);

    createBoundingBox();
}

void CMesh::setIndices(const int *indices, size_t size)
{
    mIndices.reserve(size);
    std::move(indices, indices + size, std::back_inserter(mIndices));
    mIndices.shrink_to_fit();

    mIndicesVbo.copy(mIndices);
}

void CMesh::createBoundingBox()
{
    glm::vec3 lowerBound(0);
    glm::vec3 upperBound(0);

    for (const auto &vtx : mVertices)
    {
        lowerBound = glm::min(lowerBound, vtx);
        upperBound = glm::max(upperBound, vtx);
    }

    mBBox = geometry::CBoundingBox(lowerBound, upperBound);
}