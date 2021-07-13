#include <algorithm>

#include "CStaticModel.hpp"


CStaticModel::CStaticModel(TMeshesList& meshes, TMaterialList& materials, TMaterialToMeshMap& m2m)
    : mMeshes(meshes)
    , mMaterials(materials)
    , mMatToMesh(m2m)
{
}

void CStaticModel::draw(TProgramAdapterPtr program)
{
    // for (const auto& m : mMeshes)

    for (const auto& x : mMatToMesh)
    {
        if (x.second)
        {
            program->setMaterial(std::static_pointer_cast<CPbrMaterial>(x.second));
        }

        x.first->draw(program);
    }
}

EModelType CStaticModel::getType() const
{
    return EModelType::STATIC;
}

geometry::CBoundingBox CStaticModel::getBoundingBox() const
{
    geometry::CBoundingBox bbox;

    std::for_each(mMeshes.begin(), mMeshes.end(),
                  [&bbox](const auto& m) { bbox.unite(m->getBoundingBox()); });

    return bbox;
}

TMeshesList& CStaticModel::getMeshes()
{
    return mMeshes;
}

TMaterialList& CStaticModel::getMaterials()
{
    return mMaterials;
}

TMaterialList CStaticModel::getMaterials(TMeshPtr mesh)
{

    TMaterialList materials;

    for (const auto& m : mMatToMesh)
    {
        if (m.first == mesh)
        {
            materials.push_back(m.second);
        }
    }

    return materials;
}
