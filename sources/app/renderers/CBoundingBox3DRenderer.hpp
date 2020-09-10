
#ifndef FOSS_CBOUNDINGBOX3DRENDERER_HPP
#define FOSS_CBOUNDINGBOX3DRENDERER_HPP

#include "base/renderers/C3DRendererBase.hpp"
#include "base/scene/CBufferObject.hpp"
#include "base/scene/CArrayObject.hpp"


struct SPhongMaterial;

class CBoundingBox3DRenderer : public C3DRendererBase
{
public:
    CBoundingBox3DRenderer();
    ~CBoundingBox3DRenderer() = default;

    void draw(SStaticModel3D & model) override;

private:
    CBufferObject mVertices;
    CBufferObject mIndexes;
    CArrayObject mVao;
};

#endif //FOSS_CBOUNDINGBOX3DRENDERER_HPP
