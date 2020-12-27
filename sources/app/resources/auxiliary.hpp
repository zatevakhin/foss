#pragma once

#include <boost/filesystem/path.hpp>


class CResourceLoader;
struct SPhongMaterial;

struct aiScene; // from assimp


using boost::filesystem::path;

namespace resources
{
    void loadMaterials(const path &resourceDir, CResourceLoader &resourceLoader, const aiScene &scene, std::vector<SPhongMaterial> &materials);
}
