//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_GEOMETRY_HPP
#define MIRADREAM3D_GEOMETRY_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <cfloat>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/common.hpp"
#include "defs.h"

#ifdef ENABLE_DEBUG
#include <ostream>
#endif

/**
 * @brief Vertex data used by rendering pipeline
 * A render vertex stores each instance of a vertex for which any data differs
 * Multiple render vertices may represent the same geometric vertex
 * @note This struct is stored as a contiguous float array
 */
struct RenderVertex
{
    /**
     * @brief Constructor using individual coordinates
     * @param xPos @copybrief x
     * @param yPos @copybrief y
     * @param zPos @copybrief z
     * @param xN @copybrief nx
     * @param yN @copybrief ny
     * @param zN @copybrief nz
     * @param U @copybrief u
     * @param V @copybrief v
     */
    RenderVertex(const float xPos, const float yPos, const float zPos, const float xN, const float yN, const float zN, const float U, const float V):
        x(xPos), y(yPos), z(zPos), nx(xN), ny(yN), nz(zN), u(U), v(V) {}

    /**
     * @brief Constructor using GLM vectors
     * @param vpos Position vector
     * @param vn Normal vector
     * @param vt Texture vector
     */
    RenderVertex(glm::vec3 vpos,glm::vec3 vn,glm::vec2 vt):
        x(vpos[0]), y(vpos[1]), z(vpos[2]), nx(vn[0]), ny(vn[1]), nz(vn[2]), u(vt[0]), v(vt[1]) {}

    /**
     * @brief Overloading for [] operator
     * This gives access to all coordinates in order of definition
     * @warning No out-of-bound check
     */
    const float& operator[](const size_t i) const {
        return reinterpret_cast<const float*>(this)[i];
    }

    /** @brief Compares two vertices's positions */
    bool operator==(const RenderVertex& other) const {
        return( glm::abs(x-other.x) < FLT_EPSILON    &&
                glm::abs(y-other.y) < FLT_EPSILON &&
                glm::abs(z-other.z) < FLT_EPSILON);
    }

    /** @brief Returns position as a vector */
    glm::vec3 getPosition() const
    {
        return(glm::vec3(x,y,z));
    }
    /** @brief Returns normal as a vector */
    glm::vec3 getNormal() const { return glm::vec3(nx, ny, nz); }

    /** @brief Sets normal coordinates from a vector */
    void setNormal(const glm::vec3 n) { nx = n.x; ny = n.y; nz = n.z; }

    /** @brief X Position coordinate */
    float x;
    /** @brief Y Position coordinate */
    float y;
    /** @brief Z Position coordinate */
    float z;
    /** @brief X Normal coordinate */
    float nx;
    /** @brief Y Normal coordinate */
    float ny;
    /** @brief Z Normal coordinate */
    float nz;
    /** @brief Horizontal texture coordinate */
    float u;
    /** @brief Vertical texture coordinate */
    float v;
};


constexpr uint32_t VERTEX_NB_ELEMENTS = sizeof(RenderVertex)/sizeof(float);

/** @brief A geometric vertex represents a unique position in the mesh, viewed as a node in the mesh’s connectivity graph. */
struct GeometricVertex
{
    /** @brief List of \ref RenderVertex indices associated to this geometric vertex */
    std::vector<uint32_t> vertices;
    /** @brief A half-edge originating from this geometric vertex */
    uint32_t halfEdge;
};

struct Edge
{
    uint32_t origin;
    uint32_t end;
};

using Face = std::array<uint32_t, MAX_FACESIZE>;
using Triangle = std::array<uint32_t, 3>;

/** @brief Struct containing a face with its size */
struct SizedFace
{
    uint32_t faceSize;
    Face face;
    uint32_t &operator[](const uint32_t i)
    {
        return face[i];
    }
};

/** @brief Oriented edge */
struct HalfEdge {

    /** @brief Index of next half edge within the same face */
    uint32_t next;
    /** @brief Index of previous half edge within the same face */
    uint32_t prev;
    /** @brief Index of face the half edge is on */
    uint32_t face;
    /** @brief Index of opposite HalfEdge on an adjacent face, -1 if none
     * @note Twin is unique if the mesh is 2-manifold */
    int32_t twin;
    /** @brief Index of origin Vertex */
    uint32_t origin;
    /** @brief Index of end Vertex */
    uint32_t end;

};

#ifdef ENABLE_DEBUG
inline std::ostream& operator<<(std::ostream& os, const HalfEdge &he) {
    os << he.origin << " -> " << he.end << " next "<< he.next << " twin " << he.twin << std::endl;
    return os;
}
#endif
#endif //MIRADREAM3D_GEOMETRY_HPP
