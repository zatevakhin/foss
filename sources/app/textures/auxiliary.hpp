#pragma once

#include "ITexture.hpp"


namespace textures
{

enum class ETextureWrapMode
{
    eRepeat,
    eMirroredRepeat,
    eClampToEdge
};

enum class ETextureType
{
    eTexture2D,
    eTextureCubeMap
};

unsigned int mapTextureWrapMode(ETextureWrapMode mode);

void tryBindTexture(const CTextureSharedPtr & texture, const unsigned int textureId, const ETextureType type);

}
