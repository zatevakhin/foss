#include "resources.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/auxiliary/trace.hpp"

#include "app/scene/Mesh.hpp"

#include "app/textures/CTexture2D.hpp"
#include "app/textures/CTextureCubeMap.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace
{

typedef struct
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    std::vector<unsigned int> v_indices;
    std::vector<int> vn_indices;
    std::vector<int> vt_indices;

    std::vector<tinyobj::material_t> materials;

} MyMesh;

} // namespace


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

TMeshPtr load_model(const std::filesystem::path model)
{
    tinyobj::callback_t cb;
    cb.vertex_cb = ([](void* user_data, float x, float y, float z, float w) {
        auto mesh = reinterpret_cast<MyMesh*>(user_data);
        mesh->vertices.emplace_back(x, y, z);
    });

    cb.normal_cb = ([](void* user_data, float x, float y, float z) {
        auto mesh = reinterpret_cast<MyMesh*>(user_data);
        mesh->normals.emplace_back(x, y, z);
    });

    cb.texcoord_cb = ([](void* user_data, float x, float y, float z) {
        MyMesh* mesh = reinterpret_cast<MyMesh*>(user_data);
        mesh->texcoords.emplace_back(x, y);
    });

    cb.index_cb = ([](void* user_data, tinyobj::index_t* indices, int num_indices) {
        // NOTE: the value of each index is raw value.
        // The first index starts with 1, not 0.
        // Also, 0 is set for the index value which does not exist in .obj
        MyMesh* mesh = reinterpret_cast<MyMesh*>(user_data);

        for (int i = 0; i < num_indices; ++i)
        {
            tinyobj::index_t idx = indices[i];

            if (idx.vertex_index != 0)
            {
                mesh->v_indices.push_back(idx.vertex_index - 1);
            }

            if (idx.normal_index != 0)
            {
                mesh->vn_indices.push_back(idx.normal_index - 1);
            }

            if (idx.texcoord_index != 0)
            {
                mesh->vt_indices.push_back(idx.texcoord_index - 1);
            }
        }
    });

    cb.mtllib_cb = ([](void* user_data, const tinyobj::material_t* materials, int num_materials) {
        MyMesh* mesh = reinterpret_cast<MyMesh*>(user_data);
        for (int i = 0; i < num_materials; i++)
        {
            mesh->materials.push_back(materials[i]);
        }
    });


    MyMesh mesh;

    std::string warn;
    std::string err;

    std::ifstream ifs(model);

    if (ifs.fail())
    {
        std::cerr << "file not found." << std::endl;
        return TMeshPtr(0);
    }

    tinyobj::MaterialFileReader mtlReader(model.parent_path());
    bool ret = tinyobj::LoadObjWithCallback(ifs, cb, &mesh, &mtlReader, &warn, &err);

    if (!warn.empty())
    {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }

    if (!ret)
    {
        std::cerr << "Failed to parse .obj" << std::endl;
        return TMeshPtr(0);
    }

    printf("# of vertices         = %ld\n", mesh.vertices.size());
    printf("# of normals          = %ld\n", mesh.normals.size());
    printf("# of texcoords        = %ld\n", mesh.texcoords.size());
    printf("# of vertex indices   = %ld\n", mesh.v_indices.size());
    printf("# of normal indices   = %ld\n", mesh.vn_indices.size());
    printf("# of texcoord indices   = %ld\n", mesh.vt_indices.size());
    printf("# of materials = %ld\n", mesh.materials.size());

    std::vector<Texture> textures;

    for (const auto& m : mesh.materials)
    {
        std::filesystem::path p(model.parent_path());

        if (!m.ambient_texname.empty())
        {
            spdlog::debug("m.ambient_texname: {}", m.ambient_texname);
            auto t = get_texture(p / m.ambient_texname, ETextureType::TEXTURE_2D);
            Texture tx;
            tx.texture = t;
            tx.type = TextureType::AMBIENT;
            textures.emplace_back(tx);
        }

        if (!m.diffuse_texname.empty())
        {
            spdlog::debug("m.diffuse_texname: {}", m.diffuse_texname);
            auto t = get_texture(p / m.diffuse_texname, ETextureType::TEXTURE_2D);
            Texture tx;
            tx.texture = t;
            tx.type = TextureType::DIFFUSE;
            textures.emplace_back(tx);
        }

        if (!m.specular_texname.empty())
        {
            spdlog::debug("m.specular_texname: {}", m.specular_texname);
            auto t = get_texture(p / m.specular_texname, ETextureType::TEXTURE_2D);
            Texture tx;
            tx.texture = t;
            tx.type = TextureType::SPECULAR;
            textures.emplace_back(tx);
        }

        if (!m.specular_highlight_texname.empty())
        {
            spdlog::debug("m.specular_highlight_texname: {}", m.specular_highlight_texname);
            auto t = get_texture(p / m.specular_highlight_texname, ETextureType::TEXTURE_2D);
            Texture tx;
            tx.texture = t;
            tx.type = TextureType::SPECULAR_HIGHLIGHT;
            textures.emplace_back(tx);
        }

        if (!m.bump_texname.empty())
        {
            spdlog::debug("m.bump_texname: {}", m.bump_texname);
            auto t = get_texture(p / m.bump_texname, ETextureType::TEXTURE_2D);
            Texture tx;
            tx.texture = t;
            tx.type = TextureType::BUMP;
        }

        if (!m.displacement_texname.empty())
        {
            spdlog::debug("m.displacement_texname: {}", m.displacement_texname);
            auto t = get_texture(p / m.displacement_texname, ETextureType::TEXTURE_2D);
            Texture tx;
            tx.texture = t;
            tx.type = TextureType::DISPLACEMENT;
            textures.emplace_back(tx);
        }

        if (!m.alpha_texname.empty())
        {
            spdlog::debug("m.alpha_texname: {}", m.alpha_texname);
            auto t = get_texture(p / m.alpha_texname, ETextureType::TEXTURE_2D);
            Texture tx;
            tx.texture = t;
            tx.type = TextureType::ALPHA;
            textures.emplace_back(tx);
        }

        if (!m.reflection_texname.empty())
        {
            spdlog::debug("m.reflection_texname: {}", m.reflection_texname);
            auto t = get_texture(p / m.reflection_texname, ETextureType::TEXTURE_2D);
            Texture tx;
            tx.texture = t;
            tx.type = TextureType::REFLECTION;
            textures.emplace_back(tx);
        }
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> idxs;

    for (size_t i = 0; i < mesh.vertices.size(); ++i)
    {
        Vertex v;
        v.position = mesh.vertices[i];
        v.normal = mesh.normals[i];
        v.tex_coords = mesh.texcoords[i];

        vertices.push_back(v);
    }

    return std::make_shared<Mesh>(vertices, mesh.v_indices, textures);
}

} // namespace resources
