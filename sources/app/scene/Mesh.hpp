#pragma once

#include "CVertexBufferObject.hpp"
#include "IMesh.hpp"


class Mesh : public IMesh
{
public:
    Mesh(TVboList vbos, TBoundingBoxSharedPtr bbox, GLenum mode, GLsizei count, GLenum type,
         const void* indicies, const int materialId);

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
        return mMaterialId;
    };

private:
    TVboList mVbos;
    GLenum mMode;
    GLsizei mCount;
    GLenum mType;
    const void* mIndicies;
    TBoundingBoxSharedPtr mBoundingBox;
    const int mMaterialId;
};
