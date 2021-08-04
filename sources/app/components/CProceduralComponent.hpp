#pragma once

#include "app/scene/IProceduralGenerator.hpp"
#include "app/scene/IProceduralSettings.hpp"
#include "ecs/Component.hpp"


class CProceduralComponent : public ecs::Component<CProceduralComponent>
{
public:
    CProceduralComponent(IProceduralGenerator* generator)
        : mGenerator(generator)
    {
        mGenerator->generate();
    }

    TModelPtr get() const
    {
        return mGenerator->get();
    }

    const IProceduralSettings& getSettings() const
    {
        return mGenerator->getSettings();
    }

    void setSettings(IProceduralSettings& settings)
    {
        mGenerator->setSettings(settings);
        mGenerator->generate();
    }

private:
    TProceduralGenerator mGenerator;
};
