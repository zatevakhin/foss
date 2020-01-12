
#ifndef FOSS_CORE_RESOURCES_SPHONGMATERIAL_HPP
#define FOSS_CORE_RESOURCES_SPHONGMATERIAL_HPP

#include "app/base/textures/ITexture.hpp"

#include <glm/mat4x4.hpp>


struct SPhongMaterial
{
   glm::vec4 mDiffuseColor;
   glm::vec4 mSpecularColor;
   glm::vec4 mEmissiveColor;
   
   float mShininess = 0;
   
   CTextureSharedPtr mDiffuse;
   CTextureSharedPtr mSpecular;
   CTextureSharedPtr mEmissive;
};

#endif // FOSS_CORE_RESOURCES_SPHONGMATERIAL_HPP

