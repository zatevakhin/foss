#pragma once

#include <glm/vec2.hpp>
#include "CFrameBufferObject.hpp"
#include "app/textures/CTexture2D.hpp"


class CCompleteFbo
{

public:
    CCompleteFbo(glm::ivec2 size);
    ~CCompleteFbo() = default;

    CCompleteFbo( const CCompleteFbo& ) = delete;
    CCompleteFbo& operator=( const CCompleteFbo& ) = delete;

    void bind() const;
    void unbind() const;

    const CTexture2D& getColorTexture() const;
    const CTexture2D& getDepthTexture() const;

private:
    void createTexturesAndBind();

private:
    glm::ivec2 mSize;

    CFrameBufferObject mFbo;
    CTexture2D mFboTextureRgb;
    CTexture2D mFboTextureDepth;

};
