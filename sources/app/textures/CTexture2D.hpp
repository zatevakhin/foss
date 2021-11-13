#pragma once

#include "ITexture.hpp"
#include "app/auxiliary/glm.hpp"


class CTexture2D : public ITexture
{
public:
    CTexture2D();
    ~CTexture2D() override;

    CTexture2D(const CTexture2D&) = delete;

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

    /**
     * @see ITexture::unbind()
     */
    void unbind(unsigned int texture) const override;

    /**
     * @see ITexture::id()
     */
    unsigned int id() const override;

private:
    unsigned int mTextureId;
};
