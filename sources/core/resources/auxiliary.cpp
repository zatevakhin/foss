
#include "auxiliary.hpp"
#include "SPhongMaterial.hpp"
#include "CResourceLoader.hpp"

#include <assimp/scene.h>
#include <assimp/types.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


using boost::filesystem::path;


namespace 
{


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

class CMaterialReader
{
public:
    CMaterialReader(const aiMaterial& srcMat, const path &resourceDir,
                    CResourceLoader &resourceLoader)
        : m_srcMat(srcMat)
        , m_resourceDir(resourceDir)
        , mResourceLoader(resourceLoader)
    {
    }

    /// returns normalized material color or default value
    glm::vec4 getColor(const char *key, unsigned int type, unsigned int index)
    {
        aiColor3D color(0, 0, 0);
        if (AI_SUCCESS == m_srcMat.Get(key, type, index, color))
        {
            return glm::clamp(glm::vec4(color.r, color.g, color.b, 1), glm::vec4(0), glm::vec4(1));
        }
        return glm::vec4(0);
    }

    float getFloat(const char *key, unsigned int type, unsigned int index)
    {
        float value = 0;
        if (AI_SUCCESS == m_srcMat.Get(key, type, index, value))
        {
            return value;
        }
        return 0.f;
    }

    unsigned int getUnsigned(const char *key, unsigned int type, unsigned int index)
    {
        auto value = 0U;
        if (AI_SUCCESS == m_srcMat.Get(key, type, index, value))
        {
            return value;
        }
        return 0;
    }

    CTextureSharedPtr getTexture(const char *key, unsigned int type, unsigned int index)
    {
        aiString filename;
        if (AI_SUCCESS == m_srcMat.Get(key, type, index, filename))
        {
            path abspath = m_resourceDir / filename.data;
            return mResourceLoader.loadTexture(abspath);
        }
        return nullptr;
    }

private:
    const aiMaterial& m_srcMat;
    path m_resourceDir;
    CResourceLoader & mResourceLoader;
};

}


namespace resources
{

void loadMaterials(const path &resourceDir, CResourceLoader & resourceLoader, const aiScene & scene, std::vector<SPhongMaterial> & materials)
{
    const auto DEFAULT_SHININESS = 30.f;

    materials.resize(scene.mNumMaterials);
    for (auto mi = 0U; mi < scene.mNumMaterials; ++mi)
    {
        CMaterialReader reader(*(scene.mMaterials[mi]), resourceDir, resourceLoader);
        SPhongMaterial &material = materials[mi];

        const unsigned int shadingMode = reader.getUnsigned(AI_MATKEY_SHADING_MODEL);
        if (!canBePhongShaded(shadingMode))
        {
            throw std::runtime_error("Given shading model was not implemented");
        }

        material.mShininess = reader.getFloat(AI_MATKEY_SHININESS);
        if (material.mShininess < 1.f)
        {
            material.mShininess = DEFAULT_SHININESS;
        }
        material.mDiffuse = reader.getTexture(AI_MATKEY_TEXTURE_DIFFUSE(0));
        if (!material.mDiffuse)
        {
            material.mDiffuseColor = reader.getColor(AI_MATKEY_COLOR_DIFFUSE);
        }
        material.mSpecular = reader.getTexture(AI_MATKEY_TEXTURE_SPECULAR(0));
        if (!material.mSpecular)
        {
            material.mSpecularColor = reader.getColor(AI_MATKEY_COLOR_SPECULAR);
        }
        material.mEmissive = reader.getTexture(AI_MATKEY_TEXTURE_EMISSIVE(0));
        if (!material.mEmissive)
        {
            material.mEmissiveColor = reader.getColor(AI_MATKEY_COLOR_EMISSIVE);
        }
    }
}

}
