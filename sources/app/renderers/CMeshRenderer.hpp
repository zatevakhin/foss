#pragma once

#include "app/geometry/CCubeSphere.hpp"
#include "app/renderers/C3DRendererBase.hpp"


class CMeshRenderer : public C3DRendererBase
{
public:
    CMeshRenderer() = default;
    ~CMeshRenderer() = default;

    void draw(SStaticModel3D& model) override;
    virtual void draw(const CMeshObject& mesh) override;
};
