#pragma once


#include "base/renderers/C3DRendererBase.hpp"
#include "app/base/geometry/CCubeSphere.hpp"



class CMeshRenderer : public C3DRendererBase
{
public:
    CMeshRenderer();
    ~CMeshRenderer() = default;

    void draw(SStaticModel3D & model) override;
    virtual void draw(const CMeshObject &mesh) override;

};

