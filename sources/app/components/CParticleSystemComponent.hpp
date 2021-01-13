#pragma once

#include "ecs/Component.hpp"
#include "app/scene/CParticleSystem.hpp"


class CParticleSystemComponent : public ecs::Component<CParticleSystemComponent>
{

public:
    std::shared_ptr<CParticleSystem> mParticleSystem;
};
