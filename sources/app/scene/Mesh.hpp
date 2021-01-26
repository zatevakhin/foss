#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "CVertexArrayObject.hpp"
#include "CVertexBufferObject.hpp"
#include "IMesh.hpp"
#include "app/textures/ITexture.hpp"


enum class TextureType
{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    SPECULAR_HIGHLIGHT,
    BUMP,
    DISPLACEMENT,
    ALPHA,
    REFLECTION,
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct Texture
{
    // Texture(TTextureSharedPtr& tx, TextureType ty)
    //     : texture(tx)
    //     , type(ty)
    // {
    // }

    TTextureSharedPtr texture;
    TextureType type;
};


class Mesh : public IMesh
{
public:
    Mesh(std::vector<Vertex>&, std::vector<unsigned int>&, std::vector<Texture>&);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw(TProgramSharedPtr) override;

private:
    void setup_mesh();

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    CVertexArrayObject m_vao;
    CVertexBufferObject m_vbo;
    CVertexBufferObject m_ebo;
};
