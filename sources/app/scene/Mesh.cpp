#include "Mesh.hpp"
#include "app/auxiliary/opengl.hpp"
#include "app/shading/CUniform.hpp"
#include <fmt/core.h>
#include <glm/common.hpp>
#include <string>


Mesh::Mesh(TVerticeList& vertices, TIndiceList& indices, const TPhongMaterialPtr& material)
    : m_vao()
    , m_vbo(EBufferType::eArrayBuffer)
    , m_ebo(EBufferType::eElementArrayBuffer)
    , m_vertices(vertices)
    , m_indices(indices)
    , mMaterial(material)
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

void Mesh::applyMaterial(TProgramSharedPtr program) const
{
    program->uniform("material.shininess") = mMaterial->mShininess;

    program->uniform("material.colorDiffuse") = mMaterial->mDiffuseColor;
    program->uniform("material.colorSpecular") = mMaterial->mSpecularColor;
    program->uniform("material.colorEmissive") = mMaterial->mEmissiveColor;

    auto id = 0;

    if (mMaterial->mEmissiveTexture)
    {
        glActiveTexture(GL_TEXTURE2);
        mMaterial->mEmissiveTexture->bind();
        program->uniform("material.textureEmissive") = 2;
    }

    if (mMaterial->mSpecularTexture)
    {
        glActiveTexture(GL_TEXTURE1);
        mMaterial->mSpecularTexture->bind();
        program->uniform("material.textureSpecular") = 1;
    }

    if (mMaterial->mDiffuseTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        mMaterial->mDiffuseTexture->bind();
        program->uniform("material.textureDiffuse") = 0;
    }
}

void Mesh::draw(TProgramSharedPtr program)
{
    if (mMaterial)
    {
        applyMaterial(program);
    }

    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.unbind();
}
