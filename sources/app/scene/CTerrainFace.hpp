
// #include "Mesh.hpp"

#include <functional>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

#include "app/types/lists.hpp"


constexpr int CUBE_FACES_COUNT = 6;

constexpr std::array<glm::vec3, CUBE_FACES_COUNT> CUBE_FACES_DIRECTIONS = {
    glm::vec3(0, 1, 0),  glm::vec3(0, -1, 0), glm::vec3(1, 0, 0),
    glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1),  glm::vec3(0, 0, -1),
};

class CTerrainFace
{

public:
    CTerrainFace(int resolution, glm::vec3 localUp);

    void buildMesh(std::function<glm::vec3(glm::vec3)>& shapeFilter, TVec3List& positions,
                   TVec3List& normals, TIndiceList& indices);

    void buildMesh(std::function<glm::vec3(glm::vec3)>& shapeFilter, TVerticesList& vertices,
                   TIndiceList& indices);

private:
    const int mResolution;
    const glm::vec3 mLocalUp;
    glm::vec3 mAxisA;
    glm::vec3 mAxisB;
};
