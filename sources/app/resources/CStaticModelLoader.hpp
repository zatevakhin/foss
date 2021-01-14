#pragma once

#include "CResourceLoader.hpp"
#include "app/scene/SStaticModel3D.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>


enum class SceneImportQuality
{
    Fast,
    HighQuality,
    MaxQuality,
};


class CStaticModelLoader
{
public:
    explicit CStaticModelLoader(CResourceLoader& resourceLoader);

    SStaticModel3DPtr load(const boost::filesystem::path& path);

private:
    CResourceLoader& mResourceLoader;

    static const aiScene& openScene(const boost::filesystem::path& path, Assimp::Importer& importer,
                                    SceneImportQuality quality = SceneImportQuality::HighQuality);
};
