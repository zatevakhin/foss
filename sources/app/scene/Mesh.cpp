#include "Mesh.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/shading/CUniform.hpp"
#include <fmt/core.h>
#include <string>


Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
           std::vector<Texture>& textures)
    : m_vao()
    , m_vbo(EBufferType::eArrayBuffer)
    , m_ebo(EBufferType::eElementArrayBuffer)
    , m_vertices(vertices)
    , m_indices(indices)
    , m_textures(textures)
{
    setup_mesh();
}

Mesh::~Mesh()
{
}

void Mesh::setup_mesh()
{
    m_vao.bind();
    m_vbo.copy(m_vertices);
    m_ebo.copy(m_indices);

    gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    auto normal_offset = reinterpret_cast<void*>(offsetof(Vertex, normal));
    gl::vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), normal_offset);
    auto tex_coord_offset = reinterpret_cast<void*>(offsetof(Vertex, tex_coords));
    gl::vertex_attrib_pointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), tex_coord_offset);

    m_vao.unbind();
}


void Mesh::draw(TProgramSharedPtr program)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
        // активируем соответствующий текстурный юнит перед привязкой
        // Получаем номер текстуры (значение N в diffuse_textureN)
        glActiveTexture(GL_TEXTURE0 + i);

        std::string uniformName;
        if (m_textures[i].type == TextureType::DIFFUSE)
        {
            uniformName = fmt::format("texture_diffuse{}", diffuseNr++);
        } else if (m_textures[i].type == TextureType::SPECULAR)
        {
            uniformName = fmt::format("texture_specular{}", specularNr++);
        }

        program->uniform(uniformName.c_str()) = int(i);
        m_textures[i].texture->bind();
    }
    glActiveTexture(GL_TEXTURE0);

    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.unbind();
}
