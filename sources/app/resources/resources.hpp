#pragma once

#include "app/textures/CTexture2D.hpp"
#include "auxiliary/sdl.hpp"

#include "app/scene/IModel.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <tuple>


namespace resources
{

enum class ETextureType
{
    VIRTUAL_TEXTURE_2D,
    TEXTURE_2D,
    CUBE_MAP_TEXTURE
};


TTextureSharedPtr get_texture(const std::filesystem::path path, ETextureType type);

std::stringstream get_content_from(const std::filesystem::path path);

std::tuple<std::shared_ptr<SDL_Surface>, bool> get_image(const std::filesystem::path path);
TModelPtr get_model(const std::filesystem::path model, EModelType type);


} // namespace resources
