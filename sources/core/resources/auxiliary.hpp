#ifndef FOSS_CORE_RESOURCES_AUXILIARY_HPP
#define FOSS_CORE_RESOURCES_AUXILIARY_HPP

#include <boost/filesystem/path.hpp>

class CResourceLoader;
struct SPhongMaterial;
struct aiScene;


using boost::filesystem::path;

namespace resources
{
    void loadMaterials(const path &resourceDir, CResourceLoader &resourceLoader, const aiScene &scene, std::vector<SPhongMaterial> &materials);
}

#endif //FOSS_CORE_RESOURCES_AUXILIARY_HPP
