#pragma once

#include "ITexture.hpp"
#include "auxiliary.hpp"

#include "app/auxiliary/glm.hpp"

using namespace textures;


class CTexture2D :
    public ITexture
{
public:
    CTexture2D();
    ~CTexture2D() override;

    unsigned int id() const override;

    CTexture2D(const CTexture2D&) = delete;

    glm::ivec2 size() const;

    void bind() const override;
    static void unbind();

    void setTexture(uint format, uint internalFormat, uint type, glm::ivec2 size, const void* ptr);
    void setWrapMode(ETextureWrapMode s, ETextureWrapMode t);
    void setFilter();
    void generateMipMaps() const;



private:
    uint mTextureId;
    glm::ivec2 mSize;
    bool mHasAlpha;
};
