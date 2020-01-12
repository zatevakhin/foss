
#ifndef FOSS_CTEXTURE2D_HPP
#define FOSS_CTEXTURE2D_HPP

#include "ITexture.hpp"
#include "auxiliary.hpp"

#include "app/base/auxiliary/glm.hpp"
#include "app/base/auxiliary/sdl.hpp"

#include <boost/noncopyable.hpp>


using namespace textures;


class CTexture2D :
    private boost::noncopyable,
    public ITexture
{
public:
    CTexture2D();
    ~CTexture2D() override;

    glm::ivec2 size() const;
    bool isAlpha() const;

    void bind() const override;
    static void unbind();

    void setSurface(const SDL_Surface& surface);
    void setWrapMode(ETextureWrapMode s, ETextureWrapMode t);

    inline unsigned int id()
    {
        return mTextureId;
    }

private:
    unsigned int mTextureId;
    glm::ivec2 mSize;
    bool mHasAlpha;
};

#endif //FOSS_CTEXTURE2D_HPP
