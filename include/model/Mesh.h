//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_MESH_H
#define MIRADREAM3D_MESH_H
#include "geometry.hpp"

#include "glm/vec3.hpp"

struct aiMesh;

enum halfEdgeDirection {ABC,ACB};


class Mesh
{
#ifdef TEST_HALFEDGES
    friend class PrimitiveMeshes;
#endif
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

    bool isTriangle(const uint32_t faceID) const
    {
        return getNbVertex(faceID) == 3;
    }
    bool isQuad(const uint32_t faceID) const {
        return getNbVertex(faceID) == 4;
    }

    void addVertex(const Vertex &vertex);
    void generateEdges();
    void generateHalfEdges();
    void triangulate();
    void addQuad(const Face &face);
    void addTriangle(const Face &face);

private:
    void addEdge(const Edge &edge);
    void addHalfEdge(const HalfEdge &halfEdge);
    uint8_t getNbVertex(const uint32_t faceID) const
    {
        return mVertexCountPerFace[faceID];
    }

    /**
     * @brief Returns the index which comes after vertexID in a face
     */
    int getNextIndice(const uint32_t faceID, const uint32_t vertexID) const
    {
        const int n = getNbVertex(faceID);
        for (int i = 0; i < n; i++)
        {
            if (mFaces[faceID][i] == vertexID)
            {
                if (i+1 == n) return 0;
                return i+1;
            }
        }
        return -1;
    }


    uint32_t mMaterialID=0;
    std::vector<Vertex> mVertices;
    std::vector<Edge> mEdges;
    std::vector<HalfEdge> mHalfEdges;
    std::vector<Face> mFaces;
    std::vector<Triangle> mTriangles;
    std::vector<uint8_t> mVertexCountPerFace;
    /**
     * @brief List of the first half edge index linked a certain vertex
     * @note Is size of mVertices
     */
    std::vector<uint32_t> mHalfEdgeIndexPerVertex;
    bool isTriangulated = false;

    /**
     *
     * A :
     * E : mean of points adjacents to A
     **/
    void findNormalAndOrientation(uint32_t AId ,std::vector<uint32_t> &adjacentFaces, glm::vec3 &normal, halfEdgeDirection &direction);
    glm::vec3 getNormal(Face &face,halfEdgeDirection orientation);
};
#endif //MIRADREAM3D_MESH_H

std::ostream& operator<<(std::ostream& os, const Mesh &mesh);