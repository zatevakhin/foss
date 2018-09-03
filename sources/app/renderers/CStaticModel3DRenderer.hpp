
#ifndef FOSS_CSTATICMODEL3DRENDERER_HPP
#define FOSS_CSTATICMODEL3DRENDERER_HPP

#include "core/renderers/C3DRendererBase.hpp"

struct SPhongMaterial;


class CStaticModel3DRenderer : public C3DRendererBase
{
public:
    CStaticModel3DRenderer() = default;
    ~CStaticModel3DRenderer() = default;

    void use(IShaderProgram & program) override;
    void draw(SStaticModel3D &model) override;

private:
    void applyModelView(const glm::mat4 &local);
    void applyMaterial(const SPhongMaterial &material) const;
    void bindAttributes(const geometry::SGeometryLayout &layout) const;
};


#endif //FOSS_CSTATICMODEL3DRENDERER_HPP