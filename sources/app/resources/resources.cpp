#include "resources.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/trace.hpp"

#include "app/textures/CTexture2D.hpp"
#include "app/textures/CTextureCubeMap.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>


namespace resources
{

std::stringstream get_content_from(const std::filesystem::path path)
{
    std::ifstream f(path, std::ios::binary);

    if (!f.is_open())
    {
        throw std::runtime_error(std::string("Cannot open for reading: ") + path.string());
    }

    std::stringstream stream;
    stream << f.rdbuf();

    return stream;
}

} // namespace resources
