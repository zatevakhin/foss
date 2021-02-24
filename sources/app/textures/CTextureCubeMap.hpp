#pragma once

#include "ITexture.hpp"
#include "app/auxiliary/glm.hpp"


class CTextureCubeMap : public ITexture
{
public:
    CTextureCubeMap();
    ~CTextureCubeMap() override;

    CTextureCubeMap(const CTextureCubeMap&) = delete;

    unsigned int id() const override;
    void bind() const override;

    static void unbind();

private:
    unsigned int mTextureId;
};
