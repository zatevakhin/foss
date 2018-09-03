
#ifndef FOSS_ITEXTURE_HPP
#define FOSS_ITEXTURE_HPP

#include <memory>

class ITexture;

using CTextureSharedPtr = std::shared_ptr<ITexture>;
using CTextureWeakPtr = std::weak_ptr<ITexture>;

class ITexture
{
public:
    
    virtual ~ITexture() = default;

    /**
     * @brief 
     * 
     */
    virtual void bind() const = 0;
};

#endif //FOSS_ITEXTURE_HPP
