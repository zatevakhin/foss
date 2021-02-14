#include "Mesh.hpp"
#include "app/shading/CUniform.hpp"
#include <fmt/core.h>
#include <glm/common.hpp>
#include <string>


Mesh::Mesh(TVerticeList& vertices, TIndiceList& indices, const TPhongMaterialPtr& material,
           unsigned int primitiveType)
    : m_vao()
    , m_vbo(EBufferType::eArrayBuffer)
    , m_ebo(EBufferType::eElementArrayBuffer)
    , m_vertices(vertices)
    , m_indices(indices)
    , mMaterial(material)
    , mPrimitiveType(primitiveType)
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

    mAABB = ([](const TVerticeList& vertices) -> decltype(mAABB) {
        glm::vec3 lowerBound(0);
        glm::vec3 upperBound(0);

        for (const auto& v : vertices)
        {
            lowerBound = glm::min(lowerBound, v.position);
            upperBound = glm::max(upperBound, v.position);
        }

        return geometry::CBoundingBox(lowerBound, upperBound);
    })(m_vertices);

    gl::vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    auto normal_offset = reinterpret_cast<void*>(offsetof(Vertex, normal));
    gl::vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), normal_offset);
    auto tex_coord_offset = reinterpret_cast<void*>(offsetof(Vertex, tex_coords));
    gl::vertex_attrib_pointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), tex_coord_offset);

    m_vao.unbind();
}

geometry::CBoundingBox Mesh::getBoundingBox() const
{
    return mAABB;
};

void Mesh::draw(TProgramAdapterPtr program)
{
    if (mMaterial)
    {
        program->setMaterial(mMaterial);
    }

    m_vao.bind();
    glDrawElements(mPrimitiveType, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.unbind();
}
