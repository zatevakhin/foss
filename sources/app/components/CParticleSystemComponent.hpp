#pragma once

#include "app/scene/CParticleSystem.hpp"
#include "ecs/Component.hpp"


class CParticleSystemComponent : public ecs::Component<CParticleSystemComponent>
{

public:
    std::shared_ptr<CParticleSystem> mParticleSystem;
};
