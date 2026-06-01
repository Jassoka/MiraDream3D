//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_MESH_H
#define MIRADREAM3D_MESH_H
#include "geometry.hpp"


class Mesh
{
private:
    uint32_t mMaterialID = {};
    std::vector<Vertex> mVertices;
    std::vector<Edge> mEdges;
    std::vector<HalfEdge> mHalfEdges;
    std::vector<Face> mFaces;
};
#endif //MIRADREAM3D_MESH_H
