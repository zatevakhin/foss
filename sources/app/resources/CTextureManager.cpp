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

TTextureSharedPtr CTextureManager::get(const std::string name)
{
    return mTextureCache[name];
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
    texture->setWrapMode(textures::ETextureWrapMode::eRepeat, textures::ETextureWrapMode::eRepeat);
    texture->setFilter();
    texture->setTexture(format, format, GL_UNSIGNED_BYTE, size, surface->pixels);
    texture->generateMipMaps();

    CTexture2D::unbind();
    return texture;
}

TTextureSharedPtr getTextureCubeMap(const std::filesystem::path path)
{
    auto texture = std::make_shared<CTextureCubeMap>();

    texture->bind();
    texture->setWrapMode(textures::ETextureWrapMode::eClampToEdge,
                         textures::ETextureWrapMode::eClampToEdge,
                         textures::ETextureWrapMode::eClampToEdge);

    for (auto i = 0U; i < 6; ++i)
    {
        std::string file(fmt::format("{}.{}.png", path.string(), i + 1));

        const auto [surface, is_alpha] = get_image(file);
        const GLenum format = is_alpha ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GLint(format), surface->w, surface->h,
                     0, format, GL_UNSIGNED_BYTE, surface->pixels);
    }

    texture->genMipMap();
    texture->unbind();

    return texture;
}

} // namespace TextureManagement
