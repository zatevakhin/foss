#pragma once

#include "C3DRendererBase.hpp"


class CParticleSystemRenderer : public C3DRendererBase
{
public:
    CParticleSystemRenderer();
    ~CParticleSystemRenderer() = default;

    void draw(SStaticModel3D & model) override;
    void draw(const CMeshObject& mesh) override;

};
