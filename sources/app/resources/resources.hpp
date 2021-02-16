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

std::stringstream get_content_from(const std::filesystem::path path);

} // namespace resources
