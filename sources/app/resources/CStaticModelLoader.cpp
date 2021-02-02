
#include "CStaticModelLoader.hpp"

#include "app/scene/CStaticModel.hpp"
#include "resources.hpp"


namespace
{

class CMaterialReader
{
public:
    CMaterialReader(const aiMaterial& srcMat, const std::filesystem::path& resourceDir)
        : mSrcMat(srcMat)
        , mResourceDir(resourceDir)
    {
    }

    /// returns normalized material color or default value
    glm::vec4 getColor(const char* key, unsigned int type, unsigned int index)
    {
        aiColor3D color(0, 0, 0);
        if (AI_SUCCESS == mSrcMat.Get(key, type, index, color))
        {
            return glm::clamp(glm::vec4(color.r, color.g, color.b, 1), glm::vec4(0), glm::vec4(1));
        }
        return glm::vec4(0);
    }

    float getFloat(const char* key, unsigned int type, unsigned int index)
    {
        float value = 0;
        if (AI_SUCCESS == mSrcMat.Get(key, type, index, value))
        {
            return value;
        }
        return 0.f;
    }

    unsigned int getUnsigned(const char* key, unsigned int type, unsigned int index)
    {
        auto value = 0U;
        if (AI_SUCCESS == mSrcMat.Get(key, type, index, value))
        {
            return value;
        }
        return 0;
    }

    TTextureSharedPtr getTexture(const char* key, unsigned int type, unsigned int index)
    {
        aiString filename;
        if (AI_SUCCESS == mSrcMat.Get(key, type, index, filename))
        {
            const auto abspath = mResourceDir / filename.data;
            return resources::get_texture(abspath.c_str(), resources::ETextureType::TEXTURE_2D);
        }
        return nullptr;
    }

private:
    const aiMaterial& mSrcMat;
    std::filesystem::path mResourceDir;
};

bool canBePhongShaded(unsigned shadingMode)
{
    switch (shadingMode)
    {
    case 0:
    case aiShadingMode_Phong:
    case aiShadingMode_Blinn:
    case aiShadingMode_Gouraud:
    case aiShadingMode_Flat:
        return true;
    default:
        return false;
    }
}

} // namespace


CStaticModelLoader::CStaticModelLoader(const aiScene* scene, const std::filesystem::path& path)
    : mScene(*scene)
    , mModelDirectory(path)
{
}

TModelPtr CStaticModelLoader::getModel()
{
    visitNodeTree();
    loadMaterials(); // should be before adding meshes

    for (auto i = 0U; i < mScene.mNumMeshes; ++i)
    {
        add(*(mScene.mMeshes[i]));
    }

    return std::make_shared<CStaticModel>(mMeshes);
}

void CStaticModelLoader::loadMaterials()
{
    const auto DEFAULT_SHININESS = 30.f;

    for (auto i = 0U; i < mScene.mNumMaterials; ++i)
    {
        auto& mat = mMaterials.emplace_back(new SMaterialPhong());

        const auto& material = *(mScene.mMaterials[i]);
        CMaterialReader reader(material, mModelDirectory);

        const unsigned int shadingMode = reader.getUnsigned(AI_MATKEY_SHADING_MODEL);
        if (!canBePhongShaded(shadingMode))
        {
            spdlog::error("Given shading model was not implemented");
        }

        mat->mShininess = reader.getFloat(AI_MATKEY_SHININESS);

        if (mat->mShininess < 1.f)
        {
            mat->mShininess = DEFAULT_SHININESS;
        }

        mat->mDiffuseTexture = reader.getTexture(AI_MATKEY_TEXTURE_DIFFUSE(0));
        if (mat->mDiffuseTexture)
        {
            mat->mDiffuseColor = reader.getColor(AI_MATKEY_COLOR_DIFFUSE);
        }

        mat->mSpecularTexture = reader.getTexture(AI_MATKEY_TEXTURE_SPECULAR(0));
        if (mat->mDiffuseTexture)
        {
            mat->mSpecularColor = reader.getColor(AI_MATKEY_COLOR_SPECULAR);
        }

        mat->mEmissiveTexture = reader.getTexture(AI_MATKEY_TEXTURE_EMISSIVE(0));
        if (mat->mDiffuseTexture)
        {
            mat->mSpecularColor = reader.getColor(AI_MATKEY_COLOR_EMISSIVE);
        }
    }
}


void CStaticModelLoader::visitNodeTree()
{
    visitNode(*(mScene.mRootNode), glm::mat4(1));
}

void CStaticModelLoader::visitNode(const aiNode& node, const glm::mat4& parentTransform)
{
    auto nodeTransform = glm::make_mat4(&node.mTransformation.a1);
    const glm::mat4 globalMat4 = parentTransform * glm::transpose(nodeTransform);

    for (auto mi = 0U; mi < node.mNumMeshes; ++mi)
    {
        const unsigned int meshNo = node.mMeshes[mi];

        if (mMeshTransforms.count(meshNo))
        {
            spdlog::error("Mesh #{} used twice in node tree", meshNo);
        }

        mMeshTransforms[meshNo] = globalMat4;
    }

    for (auto ci = 0U; ci < node.mNumChildren; ++ci)
    {
        visitNode(*node.mChildren[ci], globalMat4);
    }
}

void CStaticModelLoader::add(const aiMesh& mesh)
{
    TVerticeList vertices;
    TIndiceList indices;

    copyVertices(mesh, vertices);
    copyIndices(mesh, indices);

    mMeshes.emplace_back(new Mesh(vertices, indices, mMaterials[mesh.mMaterialIndex]));
}

void CStaticModelLoader::copyVertices(const aiMesh& mesh, TVerticeList& vertices) const
{
    auto numVertices = mesh.mNumVertices;
    vertices.reserve(numVertices);

    auto verticeList = mesh.mVertices;
    auto normalList = mesh.mNormals;


    auto meshHasTextureCoords = mesh.HasTextureCoords(0);
    auto texCoordList = mesh.mTextureCoords[0];

    // auto meshHasTangentsAndBitangents = mesh.HasTangentsAndBitangents();
    // auto tangentList = mesh.mTangents;
    // auto bitangentList = mesh.mBitangents;

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        const auto& position = glm::make_vec3(&verticeList[i].x);
        const auto& normal = glm::make_vec3(&normalList[i].x);
        const auto& tex_coord =
            meshHasTextureCoords ? glm::make_vec2(&texCoordList[i].x) : glm::vec2(0);

        vertices.emplace_back(position, normal, tex_coord);
    }
}

void CStaticModelLoader::copyIndices(const aiMesh& mesh, TIndiceList& indices) const
{
    auto numFaces = mesh.mNumFaces;
    auto facesList = mesh.mFaces;

    for (unsigned int i = 0; i < numFaces; ++i)
    {
        auto numIndices = facesList[i].mNumIndices;

        for (unsigned int j = 0; j < numIndices; ++j)
        {
            indices.push_back(facesList[i].mIndices[j]);
        }
    }
}
