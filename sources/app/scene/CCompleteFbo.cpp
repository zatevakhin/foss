
#include "CCompleteFbo.hpp"
#include <tuple>


CCompleteFbo::CCompleteFbo(glm::ivec2 size)
    : mSize(size)
    , mFbo()
    , mFboTextureRgb()
    , mFboTextureDepth()
{
    createTexturesAndBind();
}

void CCompleteFbo::createTexturesAndBind()
{
    mFbo.bind();

    mFboTextureRgb.bind();
    mFboTextureRgb.setTexture(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, mSize, 0);
    mFboTextureRgb.setFilter();
    mFboTextureRgb.unbind();

    mFboTextureDepth.bind();
    mFboTextureDepth.setTexture(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, mSize, 0);
    mFboTextureDepth.setFilter();
    mFboTextureDepth.unbind();

    mFbo.attachTexture(mFboTextureRgb.id(), GL_COLOR_ATTACHMENT0);
    mFbo.attachTexture(mFboTextureDepth.id(), GL_DEPTH_STENCIL_ATTACHMENT);

    mFbo.unbind();
}


void CCompleteFbo::bind() const
{
    mFbo.bind();
}

void CCompleteFbo::unbind() const
{
    mFbo.unbind();
}

const CTexture2D& CCompleteFbo::getColorTexture() const
{
    return mFboTextureRgb;
}

const CTexture2D& CCompleteFbo::getDepthTexture() const
{
    return mFboTextureDepth;
}
