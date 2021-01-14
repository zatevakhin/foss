#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <memory>


namespace
{

struct SDLSurfaceDeleter
{
    void operator()(SDL_Surface* ptr)
    {
        SDL_FreeSurface(ptr);
    }
};

} // namespace

using TSurfacePtr = std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>;
