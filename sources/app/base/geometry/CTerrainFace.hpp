#pragma once

#include "app/base/geometry/CMesh.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class CTerrainFace {

    public:
        CTerrainFace(CMesh &mesh, int resolution, glm::vec3 localUp);

        void buildMesh();

    private:
        CMesh &mMesh;
        const int mResolution;
        const glm::vec3 mLocalUp;
        glm::vec3 mAxisA;
        glm::vec3 mAxisB;
};