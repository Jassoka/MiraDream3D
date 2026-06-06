//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_GEOMETRY_HPP
#define MIRADREAM3D_GEOMETRY_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <cfloat>
#include <cmath>

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

    bool operator==(const Vertex& other) const;

    //TODO check si 2 vertices sont à la même position
};


constexpr uint32_t VERTEX_NB_ELEMENTS = sizeof(Vertex)/sizeof(float);


inline bool Vertex::operator==(const Vertex& other) const
{
    return (this->x == other.x && this->y == other.y && this->z == other.z);
    /*
    for (size_t i = 0; i < VERTEX_NB_ELEMENTS; ++i)
    {
        if (std::abs((*this)[i] - other[i]) > FLT_EPSILON)
        {
            return false;
        }
    }
    return true;*/
}

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
 * @brief Index of oppposite HalfEdge
 */
    uint32_t twin;
    /**
 * @brief Index of origin Vertex
 */
    uint32_t origin;
};

#endif //MIRADREAM3D_GEOMETRY_HPP
