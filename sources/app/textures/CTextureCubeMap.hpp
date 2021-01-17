#pragma once

#include "ITexture.hpp"
#include "app/auxiliary/glm.hpp"
#include "auxiliary.hpp"


using namespace textures;


class CTextureCubeMap : public ITexture
{
public:
    CTextureCubeMap();
    ~CTextureCubeMap() override;

    unsigned int id() const override;

    CTextureCubeMap(const CTextureCubeMap&) = delete;

    void bind() const override;
    static void unbind();

    void genMipMap();
    void setWrapMode(ETextureWrapMode s, ETextureWrapMode t, ETextureWrapMode r);

private:
    unsigned int mTextureId;
    glm::ivec2 mSize;
    bool mHasAlpha;
};
