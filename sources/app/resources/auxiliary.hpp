#pragma once

#include <filesystem>
#include <vector>

struct SPhongMaterial;

struct aiScene; // from assimp


namespace resources
{
void loadMaterials(const std::filesystem::path& resourceDir, const aiScene& scene,
                   std::vector<SPhongMaterial>& materials);
}
