//
// Created by jassoka on 7/12/26.
//
#include "model/SceneImport.h"

#include "model/MeshBuilder.h"
#include "model/Node.h"

SceneImport::SceneImport() { mLocalRootNode = new HierarchyNode(); }

SceneImport::~SceneImport() { delete mLocalRootNode; }

uint32_t SceneImport::newMesh()
{
    mLocalMeshes.push_back(Mesh());
    return mLocalMeshes.size()-1;
}

uint32_t SceneImport::newLocalMaterial(const std::string &name) { return mLocalMaterialRegistry.newMaterial(name); }

int32_t SceneImport::getLocalMaterialID(const std::string &name) { return mLocalMaterialRegistry.getMaterialID(name); }

Material& SceneImport::getLocalMaterial(const uint32_t id) { return mLocalMaterialRegistry.getMaterial(id); }

uint32_t SceneImport::getLocalTextureID(const std::string& path)
{
    const auto it = mTexturePaths.find(path);
    if (it == mTexturePaths.end())
        mTexturePaths[path] = mTextureCount++;
    return mTexturePaths[path];
}

void SceneImport::buildMesh(const uint32_t id, const MeshBuildData &data, const MeshBuildFlags &flags)
{
    MeshBuilder::build(&mLocalMeshes[id], data, flags);
}
