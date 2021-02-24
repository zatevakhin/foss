#pragma once

#include "ITexture.hpp"
#include "app/auxiliary/glm.hpp"


class CTexture2D : public ITexture
{
public:
    CTexture2D();
    ~CTexture2D() override;

    CTexture2D(const CTexture2D&) = delete;

    unsigned int id() const override;
    void bind() const override;
    static void unbind();

private:
    unsigned int mTextureId;
};
