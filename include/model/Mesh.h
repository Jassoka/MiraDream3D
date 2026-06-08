//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_MESH_H
#define MIRADREAM3D_MESH_H
#include "geometry.hpp"
#include <iostream>

#include "../../../../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/OpenGL.framework/Headers/gltypes.h"
#include "glm/vec3.hpp"

struct aiMesh;

enum halfEdgeDirection {ABC,ACB};


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


    const std::vector<Triangle>& getTriangles() const
    {
        return mTriangles;
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

private:
    uint32_t mMaterialID=0;
    std::vector<Vertex> mVertices;
    std::vector<Edge> mEdges;
    std::vector<HalfEdge> mHalfEdges;
    std::vector<Face> mFaces;
    std::vector<Triangle> mTriangles;
    std::vector<uint8_t> mVertexCountPerFace;
    bool isTriangulated = false;

    void findNormalAndOrientation(uint32_t A,std::vector<uint32_t> adjacentFaces,glm::vec3* normalPtr,halfEdgeDirection* directionPtr);
    glm::vec3 getNormal(Face &face,halfEdgeDirection orientation);
};
#endif //MIRADREAM3D_MESH_H

std::ostream& operator<<(std::ostream& os, const Mesh &mesh);