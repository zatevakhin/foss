#pragma once

#include <memory>


class ITexture
{
public:
    virtual ~ITexture() = default;

    /**
     * @brief
     *
     */
    virtual void bind() const = 0;

    virtual unsigned int id() const = 0;
};

using TTextureSharedPtr = std::shared_ptr<ITexture>;
