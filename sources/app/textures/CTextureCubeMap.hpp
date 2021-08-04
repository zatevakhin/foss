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
    void bind(unsigned int) const override;
    void bind() const override;

    void unbind() const override;

private:
    unsigned int mTextureId;
};
