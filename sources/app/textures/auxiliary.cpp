
#include "auxiliary.hpp"
#include "app/auxiliary/opengl.hpp"

#include "CTexture2D.hpp"
#include "CTextureCubeMap.hpp"

#include <map>


namespace textures
{

unsigned int mapTextureWrapMode(ETextureWrapMode mode)
{
    static const std::map<ETextureWrapMode, unsigned int> textureWrapModeMap = {
        {ETextureWrapMode::eRepeat, GL_REPEAT},
        {ETextureWrapMode::eMirroredRepeat, GL_MIRRORED_REPEAT},
        {ETextureWrapMode::eClampToEdge, GL_CLAMP_TO_EDGE},
    };

    return textureWrapModeMap.at(mode);
}

} // namespace textures
