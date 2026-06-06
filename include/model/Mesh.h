//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_MESH_H
#define MIRADREAM3D_MESH_H
#include "geometry.hpp"
#include <iostream>

struct aiMesh;

class Mesh
{
public:
    /**
     * @brief Default constructor for Mesh
     */
    Mesh(uint32_t MaterialID);
    /**
     * @brief Constructor by copy of meshAi
     * @param meshAi Instance of ASSIMP mesh
     */
    Mesh(aiMesh &meshAi);

    uint32_t getMaterialId() const
    {
        return mMaterialID;
    }

    const std::vector<Vertex>& getVertices() const
    {
        return mVertices;
    }

    const std::vector<Edge>& getEdges() const
    {
        return mEdges;
    }

    const std::vector<HalfEdge>& getHalfEdges() const
    {
        return mHalfEdges;
    }

    const std::vector<Face>& getFaces() const
    {
        return mFaces;
    }

    /**
     * @brief Prints mesh contents in the console
     */
    bool operator==(const Mesh &other) const;

    void addVertex(const Vertex &vertex);
    void addEdge(const Edge &edge);
    void addHalfEdge(const HalfEdge &halfEdge);
    void generateEdges();
    void triangulate();
    void addQuad(const Face &face);
    void addTriangle(const Face &face);
    /*
    /**
     * @brief Computes and returns a mesh sorted by hashing the vertices
     */
    //Mesh sortedMesh() const; //TODO pue la merde
private:
    uint32_t mMaterialID=0;
    std::vector<Vertex> mVertices;
    std::vector<Edge> mEdges;
    std::vector<HalfEdge> mHalfEdges;
    std::vector<Face> mFaces;
    std::vector<Triangle> mTriangles;
    std::vector<uint8_t> mVertexCountPerFace;
    bool isTriangulated = false;
};
#endif //MIRADREAM3D_MESH_H

std::ostream& operator<<(std::ostream& os, const Mesh &mesh);