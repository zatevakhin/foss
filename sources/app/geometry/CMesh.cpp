
#include "app/geometry/CMesh.hpp"


CMesh::CMesh()
    : mIndexes()
    , mVertices()
{
}

size_t CMesh::getVerticesCount() const
{
    return mVertices.size() /* * sizeof(mVertices[0]) */;
}

size_t CMesh::getIndexesCount() const
{
    return mIndexes.size();
}

const std::vector<glm::vec3>& CMesh::getVertices() const
{
    return mVertices;
}

const std::vector<int>& CMesh::getIndexes() const
{
    return mIndexes;
}

void CMesh::bindGeometry()
{
    mGeometry.copy(mVertices, mIndexes);
}

void CMesh::bind() const
{
    mGeometry.bind();
}

void CMesh::unbind() const
{
    mGeometry.unbind();
}

void CMesh::clear()
{
    mVertices.clear();
    mIndexes.clear();
}

void CMesh::setVertices(const glm::vec3 *vertices, size_t size)
{
    mVertices.reserve(size);
    std::copy(vertices, vertices + size, std::back_inserter(mVertices));

    mVertices.shrink_to_fit();
}

void CMesh::setIndexes(const int *indexes, size_t size)
{
    mIndexes.reserve(size);
    std::copy(indexes, indexes + size, std::back_inserter(mIndexes));
    mIndexes.shrink_to_fit();
}