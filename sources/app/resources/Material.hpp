#pragma once

#include "app/textures/ITexture.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <vector>

// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#reference-material
// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#reference-pbrmetallicroughness


class Material
{
public:
    Material();
    ~Material();

    int getId() const;

    std::string getName() const;
    void setName(const std::string name);

    glm::vec4 getBaseColorFactor() const;
    void setBaseColorFactor(const glm::vec4 factor);

    TTextureSharedPtr getBaseColorTexture() const;
    void setBaseColorTexture(const TTextureSharedPtr texture);

    float getMetallicFactor() const;
    void setMetallicFactor(const float factor);

    TTextureSharedPtr getMetallicTexture() const;
    void setMetallicTexture(const TTextureSharedPtr texture);

    float getRoughnessFactor() const;
    void setRoughnessFactor(const float factor);

    TTextureSharedPtr getRoughnessTexture() const;
    void setRoughnessTexture(const TTextureSharedPtr texture);

    TTextureSharedPtr getNormalTexture() const;
    void setNormalTexture(const TTextureSharedPtr texture, const double scale);

    TTextureSharedPtr getOcclusionTexture() const;
    void setOcclusionTexture(const TTextureSharedPtr texture, const double strength);

    TTextureSharedPtr getEmissiveTexture() const;
    void setEmissiveTexture(const TTextureSharedPtr texture);

    glm::vec3 getEmissiveFactor() const;
    void setEmissiveFactor(const glm::vec3 factor);


private:
    static int newId();

    std::string newName();

private:
    static int mMaterialsCount;

    int mMaterialId{0};

    std::string mName;

    glm::vec4 mBaseColorFactor{1.f};
    TTextureSharedPtr mBaseColorTexture;

    float mMetallicFactor{1.f};
    TTextureSharedPtr mMetallicTexture;

    float mRoughnessFactor{1.f};
    TTextureSharedPtr mRoughnessTexture;

    TTextureSharedPtr mNormalTexture;
    double mNormalTextureScale{1.0};

    TTextureSharedPtr mOcclusionTexture;
    double mOcclusionTextureStrength{1.0};

    TTextureSharedPtr mEmissiveTexture;
    glm::vec3 mEmissiveFactor{0.f};
};


using TMaterialSharedPtr = std::shared_ptr<Material>;
using TMaterialsList = std::vector<TMaterialSharedPtr>;
