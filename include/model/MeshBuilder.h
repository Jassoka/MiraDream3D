//
// Created by jassoka on 6/18/26.
//

#ifndef MIRADREAM3D_MESHBUILDER_H
#define MIRADREAM3D_MESHBUILDER_H
#include <vector>

#include "geometry.hpp"
#include "glm/vec3.hpp"
class Mesh;

struct MeshBuildInfo
{
    bool hasUserNormals = false;
    int nbSmoothingGroups = 0;
    bool computedFacesPerVertex = false;
    bool computedFacesAndVertices = false;
};

struct MeshBuildData
{
    std::vector<glm::vec3> positions = {};
    std::vector<SizedFace> faces = {};

    std::vector<RenderVertex> renderVertices = {};
    std::vector<GeometricVertex> geometricVertices = {};

    std::vector<Face> renderFaces = {};
    std::vector<Face> geometricFaces = {};
    std::vector<uint8_t> geometricVertexCountPerFace = {};

    std::vector<std::vector<uint32_t>> facesPerVertex = {};

    std::vector<uint8_t> smoothingGroups = {};


    std::vector<glm::vec3> userNormals = {};

    void addFace(const Face &geomFace,const Face &renderFace, const uint8_t size)
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
};

class MeshBuilder
{
public:
    explicit MeshBuilder(Mesh *mesh, MeshBuildData &data, MeshBuildInfo &flags);
    static void build(Mesh *mesh, MeshBuildData data = MeshBuildData{}, MeshBuildInfo flags = MeshBuildInfo{});

private:
    void buildImpl() const;
    void computeFacesAndVertices() const;
    Mesh *mMesh;
    MeshBuildInfo &mFlags;
    MeshBuildData &mData;

};
#endif //MIRADREAM3D_MESHBUILDER_H
