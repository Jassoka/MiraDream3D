//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_MESH_H
#define MIRADREAM3D_MESH_H
#include <iosfwd>
#include "geometry.hpp"

struct aiMesh;
class HalfEdgeBuilder;

enum halfEdgeDirection {ABC,ACB};


class Mesh
{
    friend class HalfEdgeBuilder;
public:

    /**
     * @brief Default constructor for Mesh
     */
    Mesh(uint32_t MaterialID);
    /*
    /**
     * @brief Constructor by copy of meshAi
     * @param meshAi Instance of ASSIMP mesh
     */
    //Mesh(aiMesh &meshAi);

    uint32_t getMaterialId() const
    {
        return mMaterialID;
    }

    const std::vector<RenderVertex>& getVertices() const
    {
        return mRenderVertices;
    }

    const std::vector<Edge>& getEdges() const
    {
        return mEdges;
    }

    const std::vector<HalfEdge>& getHalfEdges() const
    {
        return mHalfEdges;
    }

    uint32_t getNextHalfEdge(const uint32_t i) const
    {
        return mHalfEdges[i].next;
    }

    uint32_t getPrevHalfEdge(const uint32_t i) const
    {
        return mHalfEdges[i].prev;
    }

    int32_t getTwinHalfEdge(const uint32_t i) const
    {
        return mHalfEdges[i].twin;
    }

    const std::vector<Face>& getGeometricFaces() const
    {
        return mGeometricFaces;
    }
    const std::vector<Face>& getRenderFaces() const
    {
        return mRenderFaces;
    }

    const std::vector<Triangle>& getTriangles() const
    {
        return mTriangles;
    }


    const std::vector<uint32_t>& getComponents() const
    {
        return mComponents;
    }

     std::vector<GeometricVertex>& getGeometricVertices()
    {
        return mGeometricVertices;
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

    void addVertex(const RenderVertex &vertex);
    void addGeometricVertex(const GeometricVertex &vertex);
    //void generateEdges();
    /**
     * @brief
     * @param facesPerVertex List faces per vertex (within a geometric vertex)
     */
    void generateHalfEdges(const std::vector<std::vector<uint32_t>> *facesPerVertex = nullptr);
    void triangulate();
    void addQuad(const Face &geomFace,const Face &renderFace) ;
    void addTriangle(const Face &geomFace,const Face &renderFace) ;
private:
    void addEdge(const Edge &edge);
    void addHalfEdge(const HalfEdge &halfEdge);

    uint8_t getNbVertex(const uint32_t faceID) const
    {
        return mVertexCountPerFace[faceID];
    }

    void swapFaceOrientation(const uint32_t faceID)
    {
        Face &rf = mRenderFaces[faceID];
        Face &gf = mGeometricFaces[faceID];
        const uint32_t n = mVertexCountPerFace[faceID];
        for (uint32_t i = 0; i < n/2; i++)
        {
            const uint32_t tmp = rf[i];
            rf[i] = rf[n-i-1];
            rf[n-i-1] = tmp;

            const uint32_t tmp_ = gf[i];
            gf[i] = gf[n-i-1];
            gf[n-i-1] = tmp_;
        }
    }

    /**
     * @brief Returns the index which comes after vertexID in a face
     */
    int getNextIndice(const uint32_t faceID, const uint32_t vertexID) const
    {
        const int n = getNbVertex(faceID);
        for (int i = 0; i < n; i++)
        {
            if (mRenderFaces[faceID][i] == vertexID)
            {
                if (i+1 == n) return 0;
                return i+1;
            }
        }
        return -1;
    }


    glm::vec3 getGeometricVertexPosition(const uint32_t idx) const
    {
        return mRenderVertices[mGeometricVertices[idx].vertices[0]].toVec3();
    }

    /**
     * @brief //TODO
     **/
    halfEdgeDirection findFaceOrientation(uint32_t AId,const std::vector<uint32_t> &adjacentFaces, glm::vec3 *normalPtr = nullptr) const;
    glm::vec3 getNormal(const Face &face,halfEdgeDirection orientation) const;



    uint32_t mMaterialID=0;
    std::vector<RenderVertex> mRenderVertices;
    std::vector<GeometricVertex> mGeometricVertices;
    std::vector<Edge> mEdges;
    std::vector<HalfEdge> mHalfEdges;
    std::vector<Face> mRenderFaces;
    std::vector<Face> mGeometricFaces;
    std::vector<Triangle> mTriangles;
    std::vector<uint8_t> mVertexCountPerFace;
    std::vector<uint8_t> mSmoothingGroups;
    bool hasNormals;
    bool isSmooth;
    std::vector<glm::vec3> mHardNormals;
    std::vector<glm::vec3> mUserNormals;
    /**
     * @brief List of the first half edge index linked a certain vertex
     * @note Is size of mVertices
     */
    std::vector<uint32_t> mHalfEdgeIndexPerVertex;
    /**
     * @brief List of the first half-edge index for all components of the mesh
     */
    static halfEdgeDirection defaultHalfEdgeDirection;
    std::vector<uint32_t> mComponents;
    bool isTriangulated = false;
};
#endif //MIRADREAM3D_MESH_H

std::ostream& operator<<(std::ostream& os, const Mesh &mesh);