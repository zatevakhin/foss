#include "CTextureManager.hpp"
#include "app/auxiliary/opengl.hpp"
#include "auxiliary/sdl.hpp"

namespace
{

std::tuple<std::shared_ptr<SDL_Surface>, bool> get_image(const std::filesystem::path path)
{
    sdl::SdlImage image(path.c_str());
    return std::tuple(image.get(), image.has_alpha());
}

} // namespace


CTextureManager::CTextureManager()
{
}


namespace TextureManagement
{

TTextureSharedPtr getTexture2D(const std::filesystem::path path)
{
    const auto [surface, is_alpha] = get_image(path);
    const auto size = glm::ivec2(surface->w, surface->h);
    const auto format = is_alpha ? GL_RGBA : GL_RGB;

    auto texture = std::make_shared<CTexture2D>();

    texture->bind();

    gl::TTexParametriList params;
    // Wrap mode
    params.emplace_back(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    params.emplace_back(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filtering
    params.emplace_back(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    params.emplace_back(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gl::tex_parameteri(params);

    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE,
                 surface->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    texture->unbind();
    return texture;
}

TTextureSharedPtr getTextureCubeMap(const std::filesystem::path path)
{
    auto texture = std::make_shared<CTextureCubeMap>();

    texture->bind();
    gl::TTexParametriList params;
    // Wrap mode
    params.emplace_back(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    params.emplace_back(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    params.emplace_back(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Filtering
    params.emplace_back(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    params.emplace_back(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gl::tex_parameteri(params);

    for (auto i = 0U; i < 6; ++i)
    {
        std::string file(fmt::format("{}.{}.png", path.string(), i + 1));

        const auto [surface, is_alpha] = get_image(file);
        const GLenum format = is_alpha ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GLint(format), surface->w, surface->h,
                     0, format, GL_UNSIGNED_BYTE, surface->pixels);
    }

    texture->unbind();

    return texture;
}

} // namespace TextureManagement
