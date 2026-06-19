//
// Created by jassoka on 6/18/26.
//
#include  "model/MeshBuilder.h"
#include  "model/Mesh.h"
#include "model/MeshTopologyBuilder.h"

MeshBuilder::MeshBuilder(Mesh *mesh, MeshBuildData &data, MeshBuildFlags &flags):
    mMesh(mesh),
    mFlags(flags),
    mData(data)
{
}

void MeshBuilder::build(Mesh *mesh, MeshBuildData data, MeshBuildFlags flags)
{
    const auto builder = MeshBuilder(mesh, data, flags);
    builder.buildImpl();
}

void MeshBuilder::buildImpl() const
{
    if (!mFlags.computedFacesAndVertices)
        computeFacesAndVertices();
    mMesh->mRenderVertices = std::move(mData.renderVertices);
    mMesh->mGeometricVertices = std::move(mData.geometricVertices);
    mMesh->mRenderFaces = std::move(mData.renderFaces);
    mMesh->mGeometricFaces = std::move(mData.geometricFaces);
    mMesh->mVertexCountPerFace = std::move(mData.geometricVertexCountPerFace);

    mMesh->mHasUserNormals = mFlags.hasUserNormals;
    if (mFlags.hasUserNormals)
    {
        mMesh->mUserNormals = std::move(mData.userNormals);
    }
    mMesh->mNbSmoothingGroups = mData.nbSmoothingGroups;
    mMesh->mSmoothingGroups = mData.smoothingGroups;
    mMesh->triangulate();
    MeshTopologyBuilder::build(mMesh, mFlags.computedFacesPerVertex?&mData.facesPerVertex:nullptr); //TODO changer DES QUE POSSIBLE

    mMesh->mMaterialID=mData.materialID;

}


void MeshBuilder::computeFacesAndVertices() const
{
    mData.geometricVertices.reserve(mData.positions.size());
    mData.renderVertices.reserve(mData.positions.size()*4);
    mFlags.hasUserNormals = false;
    mData.nbSmoothingGroups = 0;
    for (auto p : mData.positions)
    {
        mData.addGeometricVertex({{}, 0});
    }
    for (auto f : mData.faces)
    {
        Face renderFace{};
        for (int i = 0; i < f.faceSize; i++) // Construction des vertices à la vollée (1 par point de face)
        {
            const uint32_t geomVertexID = f[i];
            RenderVertex v{ mData.positions[geomVertexID], glm::vec3(0.0), glm::vec2(0.0)};
            uint32_t vID = mData.addRenderVertex(v);
            mData.geometricVertices[geomVertexID].vertices.push_back(vID);
            renderFace[i] = vID;
        }
        switch (f.faceSize)
        {
        case 3:
        case 4:
            mData.addFace(f.face, renderFace, f.faceSize);
            break;
        default:
            break;
        }
    }
}
