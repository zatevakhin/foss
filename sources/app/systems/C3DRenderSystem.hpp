
#ifndef FOSS_C3DRENDERSYSTEM_HPP
#define FOSS_C3DRENDERSYSTEM_HPP

#include "app/components/CTransform3DComponent.hpp"
#include "app/components/CMeshComponent.hpp"

#include "app/shading/C3DModelProgram.hpp"
#include "app/shading/CCubeMapProgram.hpp"
#include "app/shading/CPhongProgram.hpp"

#include <anax/System.hpp>


class C3DRenderSystem
    : public anax::System<anax::Requires<CMeshComponent, CTransform3DComponent>>
{
public:
    C3DRenderSystem() = default;
    void render(const glm::mat4 & view, const glm::mat4 & projection);

private:
    void renderEnvironment(const glm::mat4 & view, const glm::mat4 & projection);
    void renderForeground(const glm::mat4 & view, const glm::mat4 & projection);

    CCubeMapProgram mEnvironmentProgram;
    C3DModelProgram mForegroundProgram;
    CPhongProgram mForegroundPhongProgram;
};

#endif //FOSS_C3DRENDERSYSTEM_HPP
