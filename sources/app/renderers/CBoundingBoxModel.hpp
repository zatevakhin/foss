#pragma once

#include "app/geometry/CMesh.hpp"


class CBoundingBoxModel
{

public:
    CBoundingBoxModel();
    ~CBoundingBoxModel() = default;

    CBoundingBoxModel(const CBoundingBoxModel&) = delete;

    void bind() const;
    void unbind() const;

private:
    CMesh mMesh;
};
