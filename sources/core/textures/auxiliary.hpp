
#ifndef FOSS_TEXTURES_HPP
#define FOSS_TEXTURES_HPP

#include "core/textures/ITexture.hpp"


namespace textures
{

enum class eTextureWrapMode
{
    Repeat,
    MirroredRepeat,
    ClampToEdge
};

enum class ETextureType
{
    eTexture2D,
    eTextureCubeMap
};

unsigned int mapTextureWrapMode(eTextureWrapMode mode);

void tryBindTexture(const CTextureSharedPtr & texture, const unsigned int textureId, const ETextureType type);


}

#endif //FOSS_TEXTURES_HPP
