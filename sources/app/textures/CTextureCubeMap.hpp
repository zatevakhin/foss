#pragma once

#include "ITexture.hpp"
#include "app/auxiliary/glm.hpp"


class CTextureCubeMap : public ITexture
{
public:
    CTextureCubeMap();
    ~CTextureCubeMap() override;

    CTextureCubeMap(const CTextureCubeMap&) = delete;

    /**
     * @see ITexture::bind()
     */
    void bind() const override;

    /**
     * @see ITexture::bind(unsigned int texture)
     */
    int bind(unsigned int texture) const override;

    /**
     * @see ITexture::unbind()
     */
    void unbind() const override;
    void unbind(unsigned int texture) const override;

    /**
     * @see ITexture::id()
     */
    unsigned int id() const override;

private:
    unsigned int mTextureId;
};
