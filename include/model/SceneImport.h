//
// Created by jassoka on 7/12/26.
//

#ifndef MIRADREAM3D_SCENEIMPORT_H
#define MIRADREAM3D_SCENEIMPORT_H
#include <map>

#include "Mesh.h"
#include "texture_types.hpp"

struct MeshBuildFlags;
struct MeshBuildData;
class Node;
/**
 * @brief Class for importing meshes, materials and textures
 * To be used by the scene manager
 */
class SceneImport
{
    friend class SceneController;
public:
    SceneImport();
    ~SceneImport();
    /** @brief Creates a new empty mesh and returns its ID */
    uint32_t newMesh();
    /** @copybrief MaterialRegistry::newMaterial */
    uint32_t newLocalMaterial(const std::string &name); //TODO PAS RENVOYER DE REFERENCE RISQUE QUE LA MEMOIRE BOUGE
    /** @getter{\ref mLocalRootNode} */
    Node *getRootNode() const { return mLocalRootNode; }
    /** @brief Build the mesh using the mesh builder at index id */
    void buildMesh(uint32_t id, const MeshBuildData &data, const MeshBuildFlags &flags);

    /** @copybrief MaterialRegistry::getMaterialID */
    int32_t getLocalMaterialID(const std::string &name);
    /**
     * @brief Returns ID to texture located at path
     * If path has been seen, then it returns existing ID
     * Else, it creates a new one
     */
    uint32_t getLocalTextureID(const std::string &path);

    /**
     * @copybrief MaterialRegistry::getMaterial
     */
    Material &getLocalMaterial(uint32_t id);

private:
    MaterialRegistry mLocalMaterialRegistry;
    std::map<std::string, uint32_t> mTexturePaths;
    int mTextureCount = 0;
    std::vector<Mesh> mLocalMeshes;
    /** @brief Temporary node under which the hierarchy is built */
    Node *mLocalRootNode;
};
#endif //MIRADREAM3D_SCENEIMPORT_H
