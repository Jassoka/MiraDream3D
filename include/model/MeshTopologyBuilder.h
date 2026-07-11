//
// Created by jassoka on 6/16/26.
//

#ifndef MIRADREAM3D_MESHTOPOLOGYBUILDER_H
#define MIRADREAM3D_MESHTOPOLOGYBUILDER_H
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "geometry.hpp"
#include "MeshBuilder.h"


class Mesh;

/**
 * @brief Class for building mesh topology
 * We consider topology to be half-edges, edges and normals for now
 * @warning Topology is only built in 2-manifold geometry
 */
class MeshTopologyBuilder
{
    friend class MeshBuilder;
    friend void MeshBuilder::buildTopology(Mesh*, const std::vector<std::vector<uint32_t>>*);
private:
    explicit MeshTopologyBuilder(Mesh *mesh, const std::vector<std::vector<uint32_t>>* facesPerVertex);
    void buildImpl();

    /**
     * @brief Returns a unique value for an unordered edge [firstVertex, secondVertex]
     * Said value is used as a key for the hashmap \ref mEdgeMap
     */
    static uint64_t getEdgeMapKey(const uint32_t firstVertex, const uint32_t secondVertex) {
        const uint32_t origin = std::min(firstVertex, secondVertex);
        const uint32_t end = std::max(firstVertex, secondVertex);
        return static_cast<uint64_t>(origin) << 32 | end;
    }

    /**
     * @brief Returns value in \ref mEdgeMap for the unordered edge [firstVertex, secondVertex]
     * Returns -1 if edge is not in the map
     */
    int32_t getEdgeMapValue(const uint32_t firstVertex, const uint32_t secondVertex) {
        const auto it = mEdgeMap.find(getEdgeMapKey(firstVertex, secondVertex));
        if (it == mEdgeMap.end()) return -1;
        return it->second;
    }

    void generateFacesPerVertex();
    void generateEdges();
    /**
     * @brief Finds a extremal vertex along the positive x-axis
     * which satisfies the condition of having at least one non-extremal neighbouring vertex
     * Returns -1 if none exist
     * (Which implies the mesh is completely flat and orthogonal to the x-axis)
     */
    int32_t findVx() const;
    /**
     * @brief Generates half-edges for a face within an unvisited component
     * @param halfEdgesToIterate Appends each new half edge to this reference
     * @return Index of face
     */
    uint32_t generateFirstFaceHalfEdges(std::vector<uint32_t> &halfEdgesToIterate);
    /**
     * @brief Generates half edges for an unvisited components
     * @param facesToVisit List of face indices
     * Which haven't been visited by the algorithm yet
     */
    void generateHalfEdges(uint32_t &facesToVisit);
    /**
     * @brief Once all half edges are generated,
     * the mesh has all their hard normals computed (if 2-manifold)
     * And each render vertex is associated with either user normals,
     * or automatically computed vertex else
     *
     * @warning If the mesh isn't 2-manifold, unspecified user normals
     * are set to 0
     */
    void generateNormals() const;

    Mesh *mMesh;

    std::vector<uint8_t> mVisitedFace;
    std::vector<uint8_t> mVisitedVertex;
    /**
     * @brief Each edge within a mesh is associated with at most 2 adjacent faces
     * Which are -1 if they haven't been yet found (or a second one doesn't exist)
     *
     * @note In 2-manifold geometry, there exists at most 2 adjacent faces per edge
     */
    std::vector<std::array<int32_t, 2>> mAdjacentFacesToEdge;
    /**
     * @brief Map which takes an unordered edge [a, b]
     * And gives its index within the mesh if it exists
     */
    std::unordered_map<uint64_t, uint32_t> mEdgeMap;
    /**
     * @brief List of computed normals for each geometric face
     */
    std::vector<glm::vec3> mNormalPerFace;
    /**
     * @brief Each edge within a mesh is associated with at most 2 half edge indices
     * Which are -1 if they haven't been yet found (or a second one doesn't exist)
     *
     * @note In 2-manifold geometry, there exists at most 2 half-edges per edge
     * (one half-edge per adjacent face)
     */
    std::vector<std::array<int32_t, 2>> mEdgeIdToHalfEdges; // Map reliant les edges à ses 2 half edges, -1 si pas encore trouvée
    /**
     * @brief List of faces per geometric vertex
     * If null, then it is automatically computed by \ref generateFacesPerVertex
     */
    const std::vector<std::vector<uint32_t>>* mFacesPerVertex = nullptr;
    /**
     * @brief If \ref mFacesPerVertex is null,
     * Then the computed vector is stored in this attribute
     */
    std::vector<std::vector<uint32_t>> mOwnedFacesPerVertex;
};

#endif //MIRADREAM3D_MESHTOPOLOGYBUILDER_H
