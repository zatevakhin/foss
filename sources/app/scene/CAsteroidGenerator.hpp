#pragma once

#include "CStaticModel.hpp"
#include "IModel.hpp"
#include "IProceduralGenerator.hpp"
#include "IProceduralSettings.hpp"
#include "app/math/noise.hpp"


class TAsteroidSettings : public IProceduralSettings
{
public:
    TAsteroidSettings(unsigned int radius, unsigned int resolution)
        : TAsteroidSettings(radius, resolution, 0U)
    {
    }

    TAsteroidSettings(unsigned int radius, unsigned int resolution, unsigned int seed)
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


class CAsteroidGenerator : public IProceduralGenerator
{
public:
    CAsteroidGenerator(IProceduralSettings& settings);

    virtual void setSettings(IProceduralSettings& settings) override;
    virtual const TAsteroidSettings& getSettings() const override;
    virtual void generate() override;
    virtual TModelPtr get() const override;

    virtual ~CAsteroidGenerator() = default;

    virtual EProceduralType getType() const override
    {
        return EProceduralType::ASTEROID;
    }

private:
    TAsteroidSettings mSettings;
    std::shared_ptr<CStaticModel> mProceduralModel;

    TMeshesList mMeshes;
    TMaterialList mMaterials;
    TMaterialToMeshMap mMaterialsToMeshes;

    Noise mNoise;
};
