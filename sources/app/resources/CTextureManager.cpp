#include "CTextureManager.hpp"
#include "app/textures/CTexture2D.hpp"
#include "app/textures/CTextureCubeMap.hpp"

#include "app/auxiliary/opengl.hpp"
#include "auxiliary/sdl.hpp"
#include <vector>

// TODO: Use GLI. (https://github.com/g-truc/gli).
namespace
{

std::tuple<std::shared_ptr<SDL_Surface>, bool> get_image(const std::filesystem::path path)
{
    sdl::SdlImage image(path.c_str());
    return std::tuple(image.get(), image.has_alpha());
}

} // namespace

template <>
void CTextureManager::getTexture(const std::filesystem::path path,
                                 std::shared_ptr<CTexture2D> texture)
{
    const auto [surface, is_alpha] = get_image(path);
    const auto size = glm::ivec2(surface->w, surface->h);
    const auto format = is_alpha ? GL_RGBA : GL_RGB;

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
}

template <>
void CTextureManager::getTexture(const std::filesystem::path path,
                                 std::shared_ptr<CTextureCubeMap> texture)
{
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

    auto parentPath = path.parent_path();
    // TODO: Refactoring.
    std::string extension = &(path.extension().string()[1]); // removing dot symbol from extension.
    auto stem = path.stem();

    for (auto i = 0U; i < 6; ++i)
    {
        auto file = parentPath / stem / fmt::format("{}.{}.png", extension, i + 1);
        const auto [surface, is_alpha] = get_image(file);
        const GLenum format = is_alpha ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GLint(format), surface->w, surface->h,
                     0, format, GL_UNSIGNED_BYTE, surface->pixels);
    }

    texture->unbind();
}

void CTextureManager::setTexture(ITexture* texture, const size_t size, const glm::vec3 color)
{
    texture->bind();
    // set texture wrap parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::vector<unsigned char> data;
    data.resize(3 * size * size);

    // unsigned char* data = new unsigned char[3 * size * size * sizeof(unsigned char)];
    for (unsigned int i = 0; i < size * size; i++)
    {
        data[i * 3] = (unsigned char)(color.x * 255.0f);
        data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
        data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    texture->unbind();
}
