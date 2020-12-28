#pragma once

#include "C3DRendererBase.hpp"
#include "CBoundingBoxModel.hpp"


class CBoundingBox3DRenderer : public C3DRendererBase
{
public:
    CBoundingBox3DRenderer();
    ~CBoundingBox3DRenderer() = default;

    void draw(SStaticModel3D & model) override;

private:
    CBoundingBoxModel mBoxModel;
};
