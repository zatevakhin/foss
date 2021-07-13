#include "Mesh.hpp"
#include "app/shading/CUniform.hpp"

#include <algorithm>
#include <fmt/core.h>
#include <glm/common.hpp>
#include <string>

namespace
{
struct SBoxFinder
{
    void operator()(const Vertex& v)
    {
        lowerBound = glm::min(lowerBound, v.position);
        upperBound = glm::max(upperBound, v.position);
    }

    geometry::CBoundingBox get() const
    {
        return geometry::CBoundingBox(lowerBound, upperBound);
    }

    glm::vec3 lowerBound{0};
    glm::vec3 upperBound{0};
};

} // namespace

Mesh::Mesh(TVerticeList& vertices, TIndiceList& indices, unsigned int primitiveType)
    : m_vao()
    , m_vbo(EBufferType::eArrayBuffer)
    , m_ebo(EBufferType::eElementArrayBuffer)
    , m_vertices(vertices)
    , m_indices(indices)
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

    mAABB = std::for_each(m_vertices.begin(), m_vertices.end(), SBoxFinder()).get();

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
    // if (mMaterial)
    // {
    //     program->setMaterial(mMaterial);
    // }

    // if (mPbrMaterial)
    // {
    //     program->setMaterial(mPbrMaterial);
    // }

    m_vao.bind();
    glDrawElements(mPrimitiveType, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.unbind();
}
