#pragma once

#include "C3DRendererBase.hpp"
#include "CBoundingBoxModel.hpp"


class CBoundingBox3DRenderer : public C3DRendererBase
{
public:
    CBoundingBox3DRenderer();
    ~CBoundingBox3DRenderer() = default;

    void draw(const CMeshObject& mesh) override;
    void draw(const TModelPtr model) override;

    void drawBoundingBox(const geometry::CBoundingBox& aabb);

private:
    CBoundingBoxModel mBoxModel;
};
