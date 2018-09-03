
#include "auxiliary.hpp"
#include "core/auxiliary/opengl.hpp"

#include "core/textures/CTexture2D.hpp"
#include "core/textures/CTextureCubeMap.hpp"

#include <map>

namespace textures
{

unsigned int mapTextureWrapMode(eTextureWrapMode mode)
{
    static const std::map<eTextureWrapMode, unsigned int> textureWrapModeMap = {
        { eTextureWrapMode::Repeat,         GL_REPEAT },
        { eTextureWrapMode::MirroredRepeat, GL_MIRRORED_REPEAT },
        { eTextureWrapMode::ClampToEdge,    GL_CLAMP_TO_EDGE },
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
