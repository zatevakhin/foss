#pragma once

#include "app/geometry/CBoundingBox.hpp"
#include "app/scene/CVertexArrayObject.hpp"
#include "app/scene/CVertexBufferObject.hpp"
#include <glm/vec3.hpp>
#include <vector>

enum class EMeshType
{
    POINTS,
    LINES,
    TRIANGLES,
    TRIANGLES_STRIP
};


class CMesh
{
public:
    CMesh();

    size_t getVerticesCount() const;
    size_t getIndicesCount() const;

    const std::vector<float>& getVertices() const;
    const std::vector<int>& getIndices() const;
    const geometry::CBoundingBox& getBoundingBox() const;

    void bind() const;
    void unbind() const;

    void clear();
    void setVertices(const float* vertices, size_t size, bool noIndices = false);
    void setIndices(const int* vertices, size_t size);

private:
    void createBoundingBox();

private:
    bool mNoIndices;

    std::vector<float> mVertices;
    std::vector<int> mIndices;

    geometry::CBoundingBox mBBox;

    CVertexBufferObject mVerticesVbo;
    CVertexBufferObject mIndicesVbo;
    CVertexArrayObject mVao;
};
