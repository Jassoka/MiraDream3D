//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_GEOMETRY_HPP
#define MIRADREAM3D_GEOMETRY_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <cfloat>
#include <ostream>

#include "glm/vec3.hpp"
#include "glm/common.hpp"
struct Vertex
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;

    /**
     * @brief Accès par indice des éléments d'un Vertex
     */
    const float& operator[](const size_t i) const {
        return reinterpret_cast<const float*>(this)[i];
    }

    bool operator==(const Vertex& other) const {
        return( glm::abs(x-other.x) < FLT_EPSILON    &&
                glm::abs(y-other.y) < FLT_EPSILON &&
                glm::abs(z-other.z) < FLT_EPSILON);
    }

    glm::vec3 toVec3() {
        return(glm::vec3(x,y,z));
    }

};


constexpr uint32_t VERTEX_NB_ELEMENTS = sizeof(Vertex)/sizeof(float);


struct Edge
{
    uint32_t origin;
    uint32_t end;
};

using Face = std::array<uint32_t, 4>;
using Triangle = std::array<uint32_t, 3>;

/**
 * @brief Oriented edge
 */
struct HalfEdge {
    uint32_t next;
    uint32_t prev;
    /**
 * @brief Face of the half edge
 */
    uint32_t face;
    /**
 * @brief Index of oppposite HalfEdge
 */
    int32_t twin;
    /**
 * @brief Index of origin Vertex
 */
    uint32_t origin;
    /**
 * @brief Index of origin Vertex
 */
    uint32_t end;

};
inline std::ostream& operator<<(std::ostream& os, const HalfEdge &he) {
    os << he.origin << " -> " << he.end << std::endl;
    return os;
}
#endif //MIRADREAM3D_GEOMETRY_HPP
