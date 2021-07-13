#pragma once

#include "CStaticModel.hpp"
#include "IModel.hpp"
#include "IProceduralGenerator.hpp"
#include "IProceduralSettings.hpp"
#include "app/math/noise.hpp"


class TPlanetSettings : public IProceduralSettings
{
public:
    TPlanetSettings(unsigned int radius, unsigned int resolution)
        : TPlanetSettings(radius, resolution, 0U)
    {
    }

    TPlanetSettings(unsigned int radius, unsigned int resolution, unsigned int seed)
        : mRadius(radius)
        , mResolution(resolution)
        , mSeed(seed)
    {
    }

    int mSeed;
    int mRadius;
    int mResolution;

    TNoiseSettings mNoiseSettings;
};


class CPlanetGenerator : public IProceduralGenerator
{
public:
    CPlanetGenerator(IProceduralSettings& settings);

    virtual void setSettings(IProceduralSettings& settings) override;
    virtual const TPlanetSettings& getSettings() const override;
    virtual void generate() override;
    virtual TModelPtr get() const override;

    virtual ~CPlanetGenerator() override = default;

    virtual EProceduralType getType() const override
    {
        return EProceduralType::PLANET;
    }

private:
    TPlanetSettings mSettings;
    std::shared_ptr<CStaticModel> mProceduralModel;

    TMeshesList mMeshes;
    TMaterialList mMaterials;
    TMaterialToMeshMap mMaterialsToMeshes;

    Noise mNoise;
};
