#pragma once

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

    virtual unsigned int id() const = 0;
};
