#pragma once

#include "IMesh.hpp"
#include "IModel.hpp"

#include <vector>


class CStaticModel : public IModel
{

public:
    explicit CStaticModel(TMeshesList& meshes);
    ~CStaticModel() = default;

    void draw(TProgramAdapterPtr program) override;
    geometry::CBoundingBox getBoundingBox() const override;
    EModelType getType() const override;
    TMeshesList& getMeshList() override;

private:
    TMeshesList mMeshes;
};
