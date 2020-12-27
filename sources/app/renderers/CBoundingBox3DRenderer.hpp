
#ifndef FOSS_CBOUNDINGBOX3DRENDERER_HPP
#define FOSS_CBOUNDINGBOX3DRENDERER_HPP

#include "app/renderers/C3DRendererBase.hpp"
#include "app/scene/CBufferObject.hpp"
#include "app/scene/CArrayObject.hpp"


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
