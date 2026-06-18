//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_MESH_H
#define MIRADREAM3D_MESH_H
#include <iosfwd>
#include "geometry.hpp"
#include "glm/fwd.hpp"

struct aiMesh;
class MeshTopologyBuilder;

enum halfEdgeDirection {ABC,ACB};


class Mesh
{
    friend class MeshTopologyBuilder;
    friend class MeshBuilder;
public:

    /**
     * @brief Default constructor for Mesh
     */
    Mesh(const uint32_t materialID): mMaterialID(materialID) {}

    Mesh(uint32_t materialID,
        const std::vector<glm::vec3> &positions,
        const std::vector<SizedFace> &faces,
        const glm::mat4 &translationRotationMatrix,
        float scale);

    uint32_t getMaterialId() const
    {
        return mMaterialID;
    }

    const std::vector<RenderVertex>& getRenderVertices() const
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

    const std::vector<GeometricVertex>& getGeometricVertices() const
    {
        return mGeometricVertices;
    }
#ifdef ENABLE_DEBUG
    bool operator==(const Mesh &other) const;
#endif

    bool isTriangle(const uint32_t faceID) const
    {
        return getNbVertex(faceID) == 3;
    }
    bool isQuad(const uint32_t faceID) const {
        return getNbVertex(faceID) == 4;
    }

private:
    void triangulate();

    uint8_t getNbVertex(const uint32_t faceID) const
    {
        return mVertexCountPerFace[faceID];
    }

    void swapFaceOrientation(uint32_t faceID);

    /**
     * @brief Returns the index which comes after vertexID in a face
     */
    int getNextIndice(uint32_t faceID, uint32_t vertexID) const;

    glm::vec3 getGeometricVertexPosition(const uint32_t idx) const
    {
        return mRenderVertices[mGeometricVertices[idx].vertices[0]].toVec3();
    }

    /**
     * @brief //TODO
     **/
    halfEdgeDirection findFaceOrientation(uint32_t AId,const std::vector<uint32_t> &adjacentFaces, glm::vec3 *normalPtr = nullptr) const;
    glm::vec3 getNormal(const Face &face,halfEdgeDirection orientation) const;

    bool isSmooth() const { return nSmoothGroups != 0; }

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
    bool mIsManifold = true;
    bool hasNormals;
    uint8_t nSmoothGroups;
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