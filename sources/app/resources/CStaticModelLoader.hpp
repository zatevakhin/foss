#pragma once

#include <assimp/scene.h>
#include <filesystem>
#include <glm/matrix.hpp>
#include <unordered_map>
#include <vector>

#include "IModelLoader.hpp"
#include "app/scene/Mesh.hpp"
#include "app/scene/PhongMaterial.hpp"


enum class EImportQuality
{
    FAST,
    HIGH,
    MAX
};


class CStaticModelLoader : public IModelLoader
{

private:
    std::unordered_map<unsigned, glm::mat4> mMeshTransforms;
    std::vector<TMeshPtr> mMeshes;
    std::vector<TPhongMaterialPtr> mMaterials;
    aiScene* mScene;
    const std::filesystem::path mModelDirectory;
    EImportQuality mQuality;

public:
    CStaticModelLoader(const std::filesystem::path& path, EImportQuality quality);

    TModelPtr getModel() override;

private:
    void loadMaterials();

    void visitNodeTree();

    void visitNode(const aiNode& node, const glm::mat4& parentTransform);

    void add(const aiMesh& mesh);

    void copyIndices(const aiMesh& mesh, TIndiceList& indices) const;
    void copyVertices(const aiMesh& mesh, TVerticeList& vertices) const;
};
