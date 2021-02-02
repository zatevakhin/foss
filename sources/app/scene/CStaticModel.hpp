#pragma once

#include "IMesh.hpp"
#include "IModel.hpp"

#include <vector>

using TMeshesList = std::vector<TMeshPtr>;

class CStaticModel : public IModel
{

public:
    explicit CStaticModel(TMeshesList& meshes);
    ~CStaticModel() = default;

    void draw(TProgramSharedPtr program) override;
    geometry::CBoundingBox getBoundingBox() const override;
    EModelType getType() const override;

private:
    TMeshesList mMeshes;
};
