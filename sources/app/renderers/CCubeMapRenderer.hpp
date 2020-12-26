
#ifndef FOSS_APP_RENDERERS_CCUBEMAPRENDERER_HPP
#define FOSS_APP_RENDERERS_CCUBEMAPRENDERER_HPP

#include "app/renderers/C3DRendererBase.hpp"


class CCubeMapRenderer : public C3DRendererBase
{
public:
    CCubeMapRenderer() = default;
    ~CCubeMapRenderer() = default;

    void draw(SStaticModel3D & model) override;

private:
    void applyModelView(const glm::mat4 & local);
    void bindAttributes(const geometry::SGeometryLayout & layout) const;

};

#endif //FOSS_APP_RENDERERS_CCUBEMAPRENDERER_HPP
