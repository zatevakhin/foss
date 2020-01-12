
#ifndef FOSS_CSTATICMODELLOADER_H
#define FOSS_CSTATICMODELLOADER_H

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "app/base/scene/SStaticModel3D.hpp"
#include "CResourceLoader.hpp"


enum class SceneImportQuality
{
    Fast,
    HighQuality,
    MaxQuality,
};


class CStaticModelLoader
{
public:
    explicit CStaticModelLoader(CResourceLoader & resourceLoader);

    SStaticModel3DPtr load(const boost::filesystem::path & path);

private:
    CResourceLoader & mResourceLoader;

    static const aiScene & openScene(
            const boost::filesystem::path & path,
            Assimp::Importer & importer,
            SceneImportQuality quality = SceneImportQuality::HighQuality
            );
};


#endif //FOSS_CSTATICMODELLOADER_H
