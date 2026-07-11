//
// Created by jassoka on 5/30/26.
//

#ifndef MIRADREAM3D_MESH_H
#define MIRADREAM3D_MESH_H
#include <iosfwd>
#include "geometry.hpp"
#include "glm/fwd.hpp"

class MeshTopologyBuilder;

/**
 * @brief Specifies the winding order used to orient a face.
 *
 * ABC means the face vertices are ordered counter-clockwise when viewed
 * from outside the mesh, so the computed normal points toward the viewer.
 *
 * ACB reverses the winding order and therefore flips the face normal.
 */
enum halfEdgeDirection {ABC,ACB};


/**
 * @brief Class for mesh data
 */
class Mesh
{
    friend class MeshTopologyBuilder;
    friend class MeshBuilder;
public:

    /** @brief Constructor for a completely empty mesh */
    Mesh() {}

    /**
     * @brief Constructor for a mesh, given a list of positions and a list of face indices
     * @param positions List of glm vectors for positions
     * @param faces List of faces using the positions vector indices
     * @param translationRotationMatrix Matrix for translation and rotation in world coordinates
     * @param scale Factor for size scaling
     */
    Mesh(const std::vector<glm::vec3> &positions,
         const std::vector<SizedFace> &faces,
         const glm::mat4 &translationRotationMatrix,
         float scale);

    /**
     * @brief Returns the material ID for a submesh index
     * @warning No check that subMeshIndex is within bounds
     */
    uint32_t getMaterialId(const uint32_t subMeshIndex) const { return mMaterials[subMeshIndex]; }

    /** @getter{\ref mRenderVertices} */
    const std::vector<RenderVertex>& getRenderVertices() const { return mRenderVertices; }

    /** @getter{\ref mEdges} */
    const std::vector<Edge>& getEdges() const { return mEdges; }

    /** @getter{\ref mHalfEdges} */
    const std::vector<HalfEdge>& getHalfEdges() const { return mHalfEdges; }

    /**
     * @brief Returns the next half-edge for half-edge at index i
     */
    uint32_t getNextHalfEdge(const uint32_t i) const { return mHalfEdges[i].next; }

    /**
     * @brief Returns the previous half-edge for half-edge at index i
     */
    uint32_t getPrevHalfEdge(const uint32_t i) const { return mHalfEdges[i].prev; }

    /**
     * @brief Returns the twin half-edge for half-edge at index i
     * -1 if none exist
     */
    int32_t getTwinHalfEdge(const uint32_t i) const { return mHalfEdges[i].twin; }

    /** @getter{\ref mGeometricFaces} */
    const std::vector<Face>& getGeometricFaces() const { return mGeometricFaces; }

    /** @getter{\ref mRenderFaces} */
    const std::vector<Face>& getRenderFaces() const { return mRenderFaces; }

    /** @brief Returns a reference to the vector of triangles for the sub-mesh at index subMeshIndex */
    const std::vector<Triangle>& getTriangles(const uint32_t subMeshIndex) const { return mTriangles[subMeshIndex]; }


    /** @getter{\ref mSubMeshIndices} */
    const std::vector<uint32_t>& getSubMeshes() const { return mSubMeshIndices; }

    /** @getter{\ref mComponents} */
    const std::vector<uint32_t>& getComponents() const { return mComponents; }

    /** @getter{\ref mGeometricVertices} */
    const std::vector<GeometricVertex>& getGeometricVertices() const { return mGeometricVertices; }

    /** @brief Returns geometric vertex position at index idx */
    glm::vec3 getGeometricVertexPosition(const uint32_t idx) const { return mRenderVertices[mGeometricVertices[idx].vertices[0]].getPosition(); }

#ifdef ENABLE_DEBUG
    bool operator==(const Mesh &other) const;
#endif

    bool isTriangle(const uint32_t faceID) const { return getNbVertex(faceID) == 3; }
    bool isQuad(const uint32_t faceID) const { return getNbVertex(faceID) == 4; }

private:
    /** @brief Triangulates the whole mesh */
    void triangulate();

    /**
     * @brief Triangulates the submesh at index subMeshIndex
     * Triangles are stored as a vector in \ref mTriangles at index subMeshIndex
     * @warning No check that subMeshIndex is within bounds
     */
    void triangulateSubMesh(uint32_t subMeshIndex);

    /** @brief Returns number of vertex for a given face faceID */
    uint8_t getNbVertex(const uint32_t faceID) const { return mVertexCountPerFace[faceID]; }

    /**
     * @brief Swaps face index orientation of face faceID
     */
    void swapFaceOrientation(uint32_t faceID);

    /** @brief Returns the index which comes after vertexID in a face */
    int getNextIndice(uint32_t faceID, uint32_t vertexID) const;

    /**
     * @brief Determines the orientation of faces adjacent to an extremal vertex.
     *
     * Uses the extremal vertex and its adjacent faces to determine the expected outward normal.
     * The selected orientation makes the faces counter-clockwise when viewed from outside the mesh,
     * in the direction maximized by the extremal vertex.
     *
     * @param AId Index of the extremal vertex.
     * @param adjacentFaces Indices of the faces adjacent to the vertex.
     * @param normalPtr Optional output pointer receiving the computed outward normal.
     * @return The required half-edge direction for the first adjacent face.
    */
    halfEdgeDirection findFaceOrientation(uint32_t AId,const std::vector<uint32_t> &adjacentFaces, glm::vec3 *normalPtr = nullptr) const;

    /**
     * @brief Returns face's normal vector, with a given face index orientation,
     * for which the face orientation is counter-clockwise when viewed from outside the mesh
     */
    glm::vec3 getNormal(const Face &face,halfEdgeDirection orientation) const;

    /** @brief Returns true if the mesh contains any smoothing group */
    bool isSmooth() const { return mNbSmoothingGroups != 0; }

    /** @brief List of material ID for each sub-mesh */
    std::vector<uint32_t> mMaterials;
    /** @brief List of render vertices */
    std::vector<RenderVertex> mRenderVertices;
    /** @brief List of geometric vertices */
    std::vector<GeometricVertex> mGeometricVertices;

    /**
     * @brief List of render face indices at which starts each sub-mesh
     * That is, the i-th sub-mesh's render faces start at \ref mSubMeshIndices [i]
     * and ends at  \ref mSubMeshIndices [i+1] - 1 within the \ref mRenderFaces vector
     */
    std::vector<uint32_t> mSubMeshIndices;

    /**
     * @brief Vector for mesh edges
     * Edges reference geometric vertices
     */
    std::vector<Edge> mEdges;
    /**
     * @brief Vector for mesh half-edges
     * Half-edges reference geometric vertices
     */
    std::vector<HalfEdge> mHalfEdges;
    /**
     * @brief Vector for mesh render faces
     * References render vertex indices
     */
    std::vector<Face> mRenderFaces;
    /**
     * @brief Vector for mesh geometric faces
     * References geometric vertex indices
     */
    std::vector<Face> mGeometricFaces;
    /** @brief Vector containing a triangle vector for each sub-mesh */
    std::vector<std::vector<Triangle>> mTriangles;
    /**
     * @brief Number of vertex per geometric face
     */
    std::vector<uint8_t> mVertexCountPerFace;
    /**
     * @brief List of smoothing group for each render vertex
     * Within a given geometric vertex, render vertices in the same smoothing group
     * have their normals averaged.
     * Smoothing group 0 means no smoothing
     */
    std::vector<uint8_t> mSmoothingGroups;
    /** @brief Flag is false if the mesh isn't a 2-manifold */
    bool mIsManifold = true;
    /** @brief Flag is true if custom normals are given by the user */
    bool mHasUserNormals = false;
    /** @brief Number of smoothing groups (excluding 0) */
    uint8_t mNbSmoothingGroups = 0;
    /**
     * @brief Vector containing computed normals for each render vertex
     * Vectors must be valid if \ref mIsManifold is true, vectors are 0 else
     */
    std::vector<glm::vec3> mHardNormals;
    /**
     * @brief Vector containing user-given normals for each render vertex
     * Vectors must be valid if \ref mHasUserNormals is true, vectors are 0 else
     */
    std::vector<glm::vec3> mUserNormals;
    /**
     * @brief List of the first half edge index linked a certain vertex
     * @note Is size of mVertices
     */
    std::vector<uint32_t> mHalfEdgeIndexPerVertex;
    /** @brief Vector containing one half-edge index for each component of the mesh */
    std::vector<uint32_t> mComponents;
    //bool mIsTriangulated = false;
};
#endif //MIRADREAM3D_MESH_H

std::ostream& operator<<(std::ostream& os, const Mesh &mesh);