//
// Created by jassoka on 6/18/26.
//

#ifndef MIRADREAM3D_MESHBUILDER_H
#define MIRADREAM3D_MESHBUILDER_H
#include <vector>
#include "geometry.hpp"
#include "glm/vec3.hpp"

class Mesh;
class MeshTopologyBuilder;

/**
 * @brief When calling \ref MeshBuilder::build ,
 * the flags specify what needs to be computed,
 * and what has already been computed by the caller
 */
struct MeshBuildFlags
{
    /**
     * @brief If true, the builder assumes the facesPerVertex vector
     * containing a list of geometric face indices for each geometric vertex
     * has been computed by the caller
     */
    bool computedFacesPerVertex = false;

    /**
     * @brief If true, the builder assumes the render/ geometric faces and vertices
     * have been computed by the caller
     */
    bool computedFacesAndVertices = false;

    /**
     * @brief If true, the materials have been assigned to sub-meshes by the caller
     * Of course, this assumes faces and vertices have been computed
     * If false, default material is applied
     */
    bool assignedMaterials = false;

    /** @brief If true, the builder expects user normals */
    bool hasUserNormals = false;
};

/**
 * @brief Mesh building data sent by \ref MeshBuilder::build caller
 * Has methods for procedural mesh building
 * Accompanied by \ref MeshBuildFlags
 */
struct MeshBuildData
{
    /** @brief List of geometric vertex positions */
    std::vector<glm::vec3> positions = {};
    /** @brief List of faces based on positions's indices */
    std::vector<SizedFace> faces = {};

    /** @brief Non-empty if \ref MeshBuildFlags::computedFacesAndVertices */
    std::vector<RenderVertex> renderVertices = {};
    /** @brief Non-empty if \ref MeshBuildFlags::computedFacesAndVertices */
    std::vector<GeometricVertex> geometricVertices = {};
    /** @brief Non-empty if \ref MeshBuildFlags::computedFacesAndVertices */
    std::vector<Face> renderFaces = {};
    /** @brief Non-empty if \ref MeshBuildFlags::computedFacesAndVertices */
    std::vector<Face> geometricFaces = {};

    /** @brief Non-empty if \ref MeshBuildFlags::computedFacesPerVertex */
    std::vector<uint8_t> geometricVertexCountPerFace = {};
    /** @brief Non-empty if \ref MeshBuildFlags::computedFacesPerVertex */
    std::vector<std::vector<uint32_t>> facesPerVertex = {};

    int nbSmoothingGroups = 0;
    std::vector<uint8_t> smoothingGroups = {};

    /** @brief Non-empty if \ref MeshBuildFlags::assignedMaterials */
    std::vector<uint32_t> subMeshFaceIndices = {};
    /** @brief Non-empty if \ref MeshBuildFlags::assignedMaterials */
    std::vector<uint32_t> materials = {};

    /** @brief Non-empty if \ref MeshBuildFlags::hasUserNormals */
    std::vector<glm::vec3> userNormals = {};


    void addFace(const Face &geomFace, const Face &renderFace, const uint8_t size)
    {
        renderFaces.push_back(renderFace);
        geometricFaces.push_back(geomFace);
        geometricVertexCountPerFace.push_back(size);
    }

    uint32_t addRenderVertex(const RenderVertex &vertex) {
        renderVertices.push_back(vertex);
        return renderVertices.size()-1;
    }
    void addGeometricVertex(const GeometricVertex &vertex) {
        geometricVertices.push_back(vertex);
    }

    uint32_t newSubMesh(const uint32_t materialID)
    {
        subMeshFaceIndices.push_back(renderFaces.size());
        materials.push_back(materialID);
        return subMeshFaceIndices.size()-1;
    }
};

/**
 * @brief Class which takes data input for mesh building
 * It is the exclusive public interface for building meshes
 */
class MeshBuilder
{
    friend class MeshTopologyBuilder;
public:
    /**
     * @brief Builds a mesh with the data provided
     * @param mesh Built mesh pointer
     * @param data Available mesh data (cf \ref MeshBuildData)
     * @param flags Flags for mesh building (cf \ref MeshBuildFlags)
     */
    static void build(Mesh *mesh, MeshBuildData data = MeshBuildData{}, MeshBuildFlags flags = MeshBuildFlags{});


private:
    /**
     * @brief Builds a mesh's topology
     * Input mesh needs to have computed vertices and faces
     * @param mesh Built mesh pointer
     * @param facesPerVertex Optional, List of geometric faces adjacent to each geometric vertex
     */
    static void buildTopology(Mesh *mesh, const std::vector<std::vector<uint32_t>>* facesPerVertex = nullptr);
    explicit MeshBuilder(Mesh *mesh, MeshBuildData &data, MeshBuildFlags &flags);
    void buildImpl() const;
    void computeFacesAndVertices() const;
    Mesh *mMesh;
    MeshBuildFlags &mFlags;
    MeshBuildData &mData;

};
#endif //MIRADREAM3D_MESHBUILDER_H
