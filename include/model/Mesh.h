//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_MESH_H
#define MIRADREAM3D_MESH_H
#include "geometry.hpp"
#include "assimp/include/assimp/mesh.h"


class Mesh
{
public:
    Mesh(uint32_t MaterialID);
    Mesh(aiMesh &meshAi);
    void printMesh();
    void addVertex(Vertex &vertex);
    void addEdge(Edge &edge);
    void addHalfEdge(HalfEdge &halfEdge);
    void addFace(Face &face);
private:
    uint32_t mMaterialID=0;
    std::vector<Vertex> mVertices;
    std::vector<Edge> mEdges;
    std::vector<HalfEdge> mHalfEdges;
    std::vector<Face> mFaces;
};
#endif //MIRADREAM3D_MESH_H
