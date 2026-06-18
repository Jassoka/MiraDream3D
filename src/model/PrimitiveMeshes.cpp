//
// Created by jassoka on 6/10/26.
//

#include "model/PrimitiveMeshes.h"
#include "model/Mesh.h"
#include "glm/mat4x4.hpp"

struct PrimitiveMesh
{
        std::vector<glm::vec3> positions;
        std::vector<SizedFace> faces;
};

Mesh PrimitiveMeshes::generateCube()
{
        return Mesh(DEFAULT_TEXTURE, cube.positions, cube.faces, glm::mat4(1.0), 1.0 );
}

/*
PrimitiveMeshes& PrimitiveMeshes::getInstance()
{
        static auto instance = PrimitiveMeshes();
        return instance;
}*/

const PrimitiveMesh PrimitiveMeshes::cube = {
        {
                {-1.0f, -1.0f, -1.0f},
                { 1.0f, -1.0f, -1.0f},
                { 1.0f,  1.0f, -1.0f},
                {-1.0f,  1.0f, -1.0f},
                {-1.0f, -1.0f,  1.0f},
                { 1.0f, -1.0f,  1.0f},
                { 1.0f,  1.0f,  1.0f},
                {-1.0f,  1.0f,  1.0f}
        },
        {
            {4, 0, 3, 2, 1},
            {4, 4, 5, 6, 7},

            {4, 0, 1, 5, 4},
            {4, 3, 7, 6, 2},

            {4, 0, 4, 7, 3},
            {4, 1, 2, 6, 5}
        }
};
