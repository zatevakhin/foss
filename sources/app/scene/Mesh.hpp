#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "CVertexArrayObject.hpp"
#include "CVertexBufferObject.hpp"
#include "IMesh.hpp"
#include "PhongMaterial.hpp"
#include "app/textures/ITexture.hpp"

struct Vertex
{
    Vertex() = default;


    Vertex(glm::vec3& p)
        : Vertex(p, glm::vec3(0.f), glm::vec2(0.f))
    {
    }

    Vertex(glm::vec3& p, glm::vec3& n)
        : Vertex(p, n, glm::vec2(0.f))
    {
    }

    Vertex(const glm::vec3& p, const glm::vec3& n, const glm::vec2& t)
        : position(p)
        , normal(n)
        , tex_coords(t)
    {
    }

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

using TVerticeList = std::vector<Vertex>;
using TIndiceList = std::vector<unsigned int>;

class Mesh : public IMesh
{
public:
    Mesh(TVerticeList& vertices, TIndiceList& indices, const TPhongMaterialPtr& material,
         unsigned int primitiveType = GL_TRIANGLES);

    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw(TProgramAdapterPtr) override;
    geometry::CBoundingBox getBoundingBox() const override;
    TPhongMaterialPtr getMaterial() override;


private:
    void setup_mesh();

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    CVertexArrayObject m_vao;
    CVertexBufferObject m_vbo;
    CVertexBufferObject m_ebo;
    TPhongMaterialPtr mMaterial;

    unsigned int mPrimitiveType;

    geometry::CBoundingBox mAABB;
};
