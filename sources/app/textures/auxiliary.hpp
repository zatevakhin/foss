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

unsigned int mapTextureWrapMode(ETextureWrapMode mode);

} // namespace textures
