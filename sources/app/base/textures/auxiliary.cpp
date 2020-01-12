
#include "auxiliary.hpp"
#include "app/base/auxiliary/opengl.hpp"

#include "CTexture2D.hpp"
#include "CTextureCubeMap.hpp"

#include <map>

namespace textures
{

unsigned int mapTextureWrapMode(ETextureWrapMode mode)
{
    static const std::map<ETextureWrapMode, unsigned int> textureWrapModeMap = {
        { ETextureWrapMode::eRepeat,         GL_REPEAT },
        { ETextureWrapMode::eMirroredRepeat, GL_MIRRORED_REPEAT },
        { ETextureWrapMode::eClampToEdge,    GL_CLAMP_TO_EDGE },
    };

    return textureWrapModeMap.at(mode);
}

void tryBindTexture(const CTextureSharedPtr & texture, const unsigned int textureId, const ETextureType type)
{
    if (nullptr != texture)
    {
        glActiveTexture(textureId);
        texture->bind();
    }
    else
    {
        switch(type)
        {
            case ETextureType::eTexture2D:
                CTexture2D::unbind();
                break;
            case ETextureType::eTextureCubeMap:
                CTextureCubeMap::unbind();
                break;
        }
    }
}


}
