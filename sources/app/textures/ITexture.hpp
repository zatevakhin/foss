#pragma once

#include <memory>


class ITexture
{
public:
    virtual ~ITexture() = default;

    /**
     * @brief bind texture to a texturing target
     */
    virtual void bind() const = 0;
    /**
     * @brief selecting active texture unit and bind texture to a texturing target
     *
     * @param texture unit
     * @return uniform texture id, which should be passed to shader
     */
    virtual int bind(unsigned int texture) const = 0;

    /**
     * @brief bind texture '0' to a texturing target
     */
    virtual void unbind() const = 0;

    virtual void unbind(unsigned int texture) const = 0;

    /**
     * @brief get texture id
     * @return generated in constructor texture name
     */
    virtual unsigned int id() const = 0;
};

using TTextureSharedPtr = std::shared_ptr<ITexture>;
