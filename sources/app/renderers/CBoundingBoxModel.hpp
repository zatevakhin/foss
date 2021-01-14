#pragma once

#include "app/scene/CVertexArrayObject.hpp"
#include "app/scene/CVertexBufferObject.hpp"


class CBoundingBoxModel
{

public:
    CBoundingBoxModel();
    ~CBoundingBoxModel() = default;

    CBoundingBoxModel(const CBoundingBoxModel&) = delete;

    void bind() const;
    void unbind() const;

private:
    CVertexArrayObject mVao;
    CVertexBufferObject mIndeciesVbo;
    CVertexBufferObject mVerteciesVbo;
};
