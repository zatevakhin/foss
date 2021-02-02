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

    Vertex(glm::vec3& p, glm::vec3& n)
        : position(p)
        , normal(n)
        , tex_coords(0)
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
    Mesh(TVerticeList& vertices, TIndiceList& indices, const TPhongMaterialPtr& material);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw(TProgramSharedPtr) override;
    void applyMaterial(TProgramSharedPtr program) const;
    geometry::CBoundingBox getBoundingBox() const override;


private:
    void setup_mesh();

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    CVertexArrayObject m_vao;
    CVertexBufferObject m_vbo;
    CVertexBufferObject m_ebo;
    TPhongMaterialPtr mMaterial;

    geometry::CBoundingBox mAABB;
};
