#pragma once

#include "IMesh.hpp"
#include "IModel.hpp"

#include <vector>


class CStaticModel : public IModel
{

public:
    CStaticModel(TMeshesList& meshes, TMaterialList& materials, TMaterialToMeshMap& m2m);
    ~CStaticModel() = default;

    void draw(TProgramAdapterPtr program) override;
    geometry::CBoundingBox getBoundingBox() const override;
    EModelType getType() const override;

    virtual TMeshesList& getMeshes() override;
    virtual TMaterialList& getMaterials() override;
    virtual TMaterialList getMaterials(TMeshPtr mesh) override;

private:
    TMeshesList mMeshes;
    TMaterialList mMaterials;
    TMaterialToMeshMap mMatToMesh;
};
