//
// Created by jassoka on 6/16/26.
//

#ifndef MIRADREAM3D_HALFEDGEBUILDER_H
#define MIRADREAM3D_HALFEDGEBUILDER_H
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "geometry.hpp"


class Mesh;

class MeshTopologyBuilder
{
public:
    static void build(Mesh *mesh, const std::vector<std::vector<uint32_t>>* facesPerVertex = nullptr);
private:
    explicit MeshTopologyBuilder(Mesh *mesh, const std::vector<std::vector<uint32_t>>* facesPerVertex);
    void buildImpl();
    static uint64_t getEdgeMapKey(const uint32_t firstVertex, const uint32_t secondVertex) {
        const uint32_t origin = std::min(firstVertex, secondVertex);
        const uint32_t end = std::max(firstVertex, secondVertex);
        return static_cast<uint64_t>(origin) << 32 | end;
    }

    int32_t getEdgeMapValue(const uint32_t firstVertex, const uint32_t secondVertex) {
        const auto it = mEdgeMap.find(getEdgeMapKey(firstVertex, secondVertex));
        if (it == mEdgeMap.end()) return -1;
        return it->second;
    }

    void generateFacesPerVertex();
    void generateEdges();
    int32_t findVx() const;
    uint32_t generateFirstFaceHalfEdges(std::vector<uint32_t> &halfEdgesToIterate);
    void generateHalfEdges(uint32_t &facesToVisit);
    void generateNormals() const;

    Mesh *mMesh;

    std::vector<uint8_t> mVisitedFace;
    std::vector<uint8_t> mVisitedVertex;
    std::vector<std::array<int32_t, 2>> mAdjacentFacesToEdge;
    std::unordered_map<uint64_t, uint32_t> mEdgeMap;
    std::vector<glm::vec3> mNormalPerFace;
    std::vector<std::array<int32_t, 2>> mEdgeIdToHalfEdges; // Map reliant les edges à ses 2 half edges, -1 si pas encore trouvée
    const std::vector<std::vector<uint32_t>>* mFacesPerVertex = nullptr;
    std::vector<std::vector<uint32_t>> mOwnedFacesPerVertex;
};

#endif //MIRADREAM3D_HALFEDGEBUILDER_H
