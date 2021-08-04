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
    void bind(unsigned int texture) const override;
    void bind() const override;
    void unbind() const override;

private:
    unsigned int mTextureId;
};
