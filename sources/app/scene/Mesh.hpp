#pragma once

#include "CVertexBufferObject.hpp"
#include "IMesh.hpp"

struct MeshInfo
{
    /**
     * @brief what kind of primitives to render.
     * Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES,
     * GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
     * GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are
     * accepted.
     */
    GLenum mode;

    /**
     * @brief the number of elements to be rendered.
     */
    GLsizei count;

    /**
     * @brief the type of the values in indices.
     * Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
     */
    GLenum type;

    /**
     * @brief a pointer to the location where the indices are stored.
     */
    void* indicies{nullptr};

    /**
     * @brief id of material which bound to this mesh.
     */
    int materialId{-1};
};

class Mesh : public IMesh
{
public:
    Mesh(TVboList vbos, TBoundingBoxSharedPtr bbox, MeshInfo& meshInfo);

    ~Mesh() override = default;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw(TProgramAdapterPtr) override;

    TBoundingBoxSharedPtr getBoundingBox() const override
    {
        return mBoundingBox;
    }

    void free()
    {
        mVbos.clear();
    }

    int getMaterialId() override
    {
        return mMeshInfo.materialId;
    };

private:
    TVboList mVbos;
    TBoundingBoxSharedPtr mBoundingBox;

    MeshInfo mMeshInfo;
};
