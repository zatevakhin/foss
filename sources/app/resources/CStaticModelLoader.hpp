#pragma once

#include "app/scene/SStaticModel3D.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <filesystem>

enum class SceneImportQuality
{
    Fast,
    HighQuality,
    MaxQuality,
};


class CStaticModelLoader
{
public:
    explicit CStaticModelLoader();

    SStaticModel3DPtr load(const std::filesystem::path& path);

private:
    static const aiScene& openScene(const std::filesystem::path& path, Assimp::Importer& importer,
                                    SceneImportQuality quality = SceneImportQuality::HighQuality);
};
