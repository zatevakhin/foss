#include <algorithm>

#include "CStaticModel.hpp"


CStaticModel::CStaticModel(TMeshesList& meshes)
    : mMeshes(meshes)
{
}

void CStaticModel::draw(TProgramAdapterPtr program)
{
    for (const auto& m : mMeshes)
    {
        m->draw(program);
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

TMeshesList& CStaticModel::getMeshList()
{
    return mMeshes;
}
