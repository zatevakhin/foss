#pragma once

#include "CVertexArrayObject.hpp"
#include "IMesh.hpp"
#include "IModel.hpp"

#include <algorithm>


class Model : public IModel
{

public:
    Model(TVaoSharedPtr vao, TMeshesList meshes, TMaterialsList materials)
        : mVao(vao)
        , mMeshes(meshes)
        , mMaterials(materials)
        , mBoundingBox(std::make_shared<CBoundingBox>())
    {
        std::vector<TMeshPtr> boundingBoxes;

        std::copy_if(mMeshes.begin(), mMeshes.end(), std::back_inserter(boundingBoxes),
                     [](TMeshPtr mesh) -> bool { return nullptr != mesh->getBoundingBox(); });

        std::for_each(boundingBoxes.begin(), boundingBoxes.end(),
                      [this](const auto& m) { mBoundingBox->unite(m->getBoundingBox()); });
    }

    ~Model() = default;

    void draw(TProgramAdapterPtr program) override
    {
        mVao->bind();

        const auto meshesCount = mMeshes.size();
        for (size_t i = 0; i < meshesCount; ++i)
        {
            const auto& mesh = mMeshes[i];
            auto x = mesh->getMaterialId();

            TMaterialSharedPtr mat = nullptr;
            if (x >= 0)
            {
                mat = mMaterials[x];
            }

            program->setMaterial(mat);
            mesh->draw(program);
            program->unsetMaterial(mat);
        }
        mVao->unbind();
    }

    TBoundingBoxSharedPtr getBoundingBox() const override
    {
        return mBoundingBox;
    }

    EModelType getType() const override
    {
        return EModelType::STATIC;
    }

    virtual TMeshesList& getMeshes() override
    {
        return mMeshes;
    }

    // virtual TMaterialList& getMaterials() override
    // {
    //     return mMaterialsOld;
    // }

private:
    TVaoSharedPtr mVao;
    TMeshesList mMeshes;
    TMaterialsList mMaterials;
    TBoundingBoxSharedPtr mBoundingBox;
};
