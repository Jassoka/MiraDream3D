//
// Created by jassoka on 6/3/26.
//

#ifndef MIRADREAM3D_MESHCOMPARISON_H
#define MIRADREAM3D_MESHCOMPARISON_H

#include <algorithm>
#include <cstdint>
#include "model/geometry.hpp"
#include "model/Mesh.h"

class MeshComparison
{
public:

    /*
    /**
     * @brief Basic hash function for a vertex (only position dependant)
     */
    /*
    static size_t hashVertex(const Vertex& v) {
        const size_t hX = std::hash<int>{}(static_cast<int>(std::round(v.x * 1000.0f)));
        const size_t hY = std::hash<int>{}(static_cast<int>(std::round(v.y * 1000.0f)));
        const size_t hZ = std::hash<int>{}(static_cast<int>(std::round(v.z * 1000.0f)));

        return (hX * 73856093) ^ (hY * 19349663) ^ (hZ * 83492791);
    }
    */

    /**
     * @brief Copies a mesh and sorts its vertices by position
     */
    static Mesh canonicalMesh(const Mesh &m)
    {
        auto vertices = m.getVertices();
        uint32_t size = vertices.size();

        std::vector<uint32_t> sortedIndices(size);
        for (uint32_t i = 0; i < size; i++) sortedIndices[i] = i;

        std::sort(sortedIndices.begin(), sortedIndices.end(), [&](uint32_t i, uint32_t j) {
            const auto& v1 = vertices[i];
            const auto& v2 = vertices[j];
            auto round = [](float f) { return std::round(f * 1000.0f); };
            if (round(v1.x) != round(v2.x)) return round(v1.x) < round(v2.x);
            if (round(v1.y) != round(v2.y)) return round(v1.y) < round(v2.y);
            return round(v1.z) < round(v2.z);
        });

        std::vector<uint32_t> oldToNew(size);
        for (uint32_t newIndex = 0; newIndex < size; ++newIndex) {
            oldToNew[sortedIndices[newIndex]] = newIndex;
        }

        Mesh sortedMesh = Mesh(m.getMaterialId());

        // CORRECTION 1 : On ajoute les sommets dans l'ordre du tri
        for (uint32_t i = 0; i < size; i++)
        {
            sortedMesh.addVertex(vertices[sortedIndices[i]]);
        }
    /*
        // CORRECTION 2 : On traduit les indices des faces
        for (const auto& f : m.getRenderFaces())
        {
            Face newRenderFace;
            for (uint32_t i = 0; i < f.size(); i++)
            {
                uint32_t oldVertexIdx = f[i]; // L'index du sommet original
                newFace[i] = oldToNew[oldVertexIdx]; // Conversion avec la table
            }
            if (f.size()==3) {
                sortedMesh.addQuad(newFace);
            }

        }
        return sortedMesh;
    }

};


#endif //MIRADREAM3D_MESHCOMPARISON_H
