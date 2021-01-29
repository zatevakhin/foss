#include "resources.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/trace.hpp"

#include "app/textures/CTexture2D.hpp"
#include "app/textures/CTextureCubeMap.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>


namespace
{

TTextureSharedPtr get_texture_2d(const std::filesystem::path path)
{
    const auto [surface, is_alpha] = resources::get_image(path);
    const auto size = glm::ivec2(surface->w, surface->h);
    const auto format = is_alpha ? GL_RGBA : GL_RGB;

    auto texture = std::make_shared<CTexture2D>();

    texture->bind();
    texture->setWrapMode(textures::ETextureWrapMode::eRepeat, textures::ETextureWrapMode::eRepeat);
    texture->setFilter();
    texture->setTexture(format, format, GL_UNSIGNED_BYTE, size, surface->pixels);
    texture->generateMipMaps();

    CTexture2D::unbind();
    return texture;
}

TTextureSharedPtr get_texture_cube_map(const std::filesystem::path path)
{
    auto texture = std::make_shared<CTextureCubeMap>();

    texture->bind();
    texture->setWrapMode(textures::ETextureWrapMode::eClampToEdge,
                         textures::ETextureWrapMode::eClampToEdge,
                         textures::ETextureWrapMode::eClampToEdge);

    for (auto i = 0U; i < 6; ++i)
    {
        std::string file(fmt::format("{}.{}.png", path.string(), i + 1));

        const auto [surface, is_alpha] = resources::get_image(file);
        const GLenum format = is_alpha ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GLint(format), surface->w, surface->h,
                     0, format, GL_UNSIGNED_BYTE, surface->pixels);
    }

    texture->genMipMap();
    texture->unbind();

    return texture;
}

} // namespace


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


TTextureSharedPtr get_texture(const std::filesystem::path path, ETextureType type)
{
    TTextureSharedPtr texture(nullptr);

    switch (type)
    {
    case ETextureType::TEXTURE_2D:
        texture = get_texture_2d(path);
        break;
    case ETextureType::CUBE_MAP_TEXTURE:
        texture = get_texture_cube_map(path);
        break;
    default:
        break;
    }

    return texture;
}


std::tuple<std::shared_ptr<SDL_Surface>, bool> get_image(const std::filesystem::path path)
{
    sdl::SdlImage image(path.c_str());
    return std::tuple(image.get(), image.has_alpha());
}

} // namespace resources
