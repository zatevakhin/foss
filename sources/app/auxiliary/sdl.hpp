#pragma once

#include "app/auxiliary/trace.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace
{

void sdl_free_surface(void* ptr)
{
    spdlog::debug("SDL_FreeSurface({:p})", ptr);
    SDL_FreeSurface(static_cast<SDL_Surface*>(ptr));
}

} // namespace


namespace sdl
{

class SdlImage
{

public:
    explicit SdlImage(const char* path)
        : m_surface(nullptr)
        , m_has_alpha(false)
    {
        m_surface.reset(IMG_Load(path), sdl_free_surface);

        if (nullptr == m_surface)
        {
            throw std::runtime_error(SDL_GetError());
        }

        m_has_alpha = SDL_ISPIXELFORMAT_ALPHA(m_surface->format->format);
        const auto format = m_has_alpha ? SDL_PIXELFORMAT_ABGR8888 : SDL_PIXELFORMAT_RGB24;

        if (m_surface->format->format != format)
        {
            m_surface.reset(SDL_ConvertSurfaceFormat(m_surface.get(), format, 0), sdl_free_surface);
        }
    }

    ~SdlImage() = default;

    SdlImage(const SdlImage&) = delete;
    SdlImage& operator=(const SdlImage&) = delete;

    std::shared_ptr<SDL_Surface> get() const
    {
        return m_surface;
    }

    bool has_alpha() const
    {
        return m_has_alpha;
    }

private:
    std::shared_ptr<SDL_Surface> m_surface;
    bool m_has_alpha;
};

} // namespace sdl
