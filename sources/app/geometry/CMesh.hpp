#pragma once

#include "app/geometry/CSimpleGeometry.hpp"
#include <glm/vec3.hpp>
#include <vector>

enum class EMeshType {
    POINTS,
    LINES,
    TRIANGLES,
    TRIANGLES_STRIP
};


class CMesh 
{
    public:
        CMesh();

        size_t getVerticesCount() const;
        size_t getIndexesCount() const;

        const glm::vec3 * getVertices() const;
        const int * getIndexes() const;

        void bindGeometry();

        void bind() const;
        void unbind() const;

        void clear();
        void setVertices(const glm::vec3 *vertices, size_t size);
        void setIndexes(const int *vertices, size_t size);


        std::vector<glm::vec3> mVertices;
        std::vector<int> mIndexes;
    private:

        geometry::CSimpleGeometry mGeometry;
};
