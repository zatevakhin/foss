#pragma once

#include "IModel.hpp"
#include "IProceduralGenerator.hpp"
#include "IProceduralSettings.hpp"
#include "app/math/noise.hpp"

#include <functional>


class TSpaceBodySettings : public IProceduralSettings
{
public:
    TSpaceBodySettings(unsigned int radius, unsigned int resolution)
        : TSpaceBodySettings(radius, resolution, 0U)
    {
    }

    TSpaceBodySettings(unsigned int radius, unsigned int resolution, unsigned int seed)
        : mRadius(radius)
        , mResolution(resolution)
        , mSeed(seed)
    {
    }

    int mSeed;
    int mRadius;
    int mResolution;

    TNoiseSettings mNoiseSettings;
    TNoiseSettings mNoiseSettings2;
    TNoiseSettings mNoiseSettings3;
};


class CSpaceBodyGenerator : public IProceduralGenerator
{
public:
    CSpaceBodyGenerator(IProceduralSettings& settings);

    virtual void setSettings(IProceduralSettings& settings) override;
    virtual const TSpaceBodySettings& getSettings() const override;
    virtual void generate() override;
    virtual TModelPtr get() const override;

    virtual ~CSpaceBodyGenerator() = default;

private:
    void generate2(std::function<glm::vec3(glm::vec3)> shapeFilter);

    TSpaceBodySettings mSettings;
    TModelPtr mProceduralModel;

    TMeshesList mMeshes;
    TMaterialsList mMaterials;

    Noise mNoise;
};
