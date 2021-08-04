#pragma once

#include "IModel.hpp"
#include "IProceduralSettings.hpp"

class IProceduralGenerator
{
public:
    virtual void setSettings(IProceduralSettings& settings) = 0;
    virtual const IProceduralSettings& getSettings() const = 0;
    virtual void generate() = 0;

    virtual TModelPtr get() const = 0;

    virtual ~IProceduralGenerator(){};
};

using TProceduralGenerator = std::unique_ptr<IProceduralGenerator>;
