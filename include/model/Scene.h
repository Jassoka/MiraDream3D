//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_SCENE_H
#define MIRADREAM3D_SCENE_H
#include <map>

#include "Camera.h"
#include "defs.h"
#include "Mesh.h"
#include "texture_types.hpp"

class Node;


/**
 * @brief Class for the main scene
 * The scene owns all meshes, materials and textures
 */
class Scene
{
    friend class SceneController;
public:
    Scene();
    ~Scene();
    Node* getRootNode() const { return mRootNode; }
    /** @getter{mMeshList} */
    const std::vector<Mesh> &getMeshes() const;
    /** @brief Deletes all meshes from the scene */
    void clearScene();
    /** @brief Adds a node as a child of the root node */
    void addNode(Node* nodePtr) const;
    /** @brief Adds a new mesh to the scene */
    void addMesh(const Mesh &mesh);
    /**
     * @brief Loads an image using QT and imports it as a texture
     * @return New texture ID, old texture ID if already exists, -1 if import failed
     */
    int32_t loadQTImageAsTexture(const QString &path);
    /** @brief Return a pointer to the raw data of texture indexed textureID */
    const uint8_t *getTextureData(const uint32_t textureID) const {return mTextureList[textureID].data();}
    /** @brief Adds a new material to the scene */
    void addMaterial(const Material &material);
    /** @brief Creates a new empty mesh in the scene and returns its pointer */
    Mesh *newMesh();

    /** @brief Deletes the last mesh from the scene */
    void removeLastMesh();

    /** @copybrief MaterialRegistry::newMaterial */
    uint32_t createNewMaterial(const std::string &name, const Material &material = Material()) { return mMaterialRegistry.newMaterial(name, material); }

    /** @copybrief MaterialRegistry::getMaterial */
    const Material &getMaterial(const uint32_t id) const
    {
        const Material &material =  mMaterialRegistry.getMaterial(id);
        return material;
    }

    /**
     * @brief Returns ID to material in given path
     * If texture has already been imported, it returns existing ID
     * Else it tries to import the file and creates a new texture
     *
     * If file fails to open, returns -1
     */
    int32_t getTextureId(const std::string & path);

private:
    /** @brief Scene camera(s) for final rendering */
    Camera mSceneCamera; //TODO: plusieurs caméras
    /** @brief Scene light(s) source(s) */
    glm::vec3 mLight; //TODO: plusieurs lumières
    /** @brief Root node in the hierarchy tree */
    Node *mRootNode = nullptr;
    /** @brief List of meshes in scene */
    std::vector<Mesh> mMeshList;
    /** @brief List of textures in scene */
    std::vector<Texture> mTextureList;
    /** @brief Map associating a path to each Texture ID */
    std::map<std::string, uint32_t> mTexturePaths;
    /** @brief Registry object for managing materials */
    MaterialRegistry mMaterialRegistry;

};

#endif //MIRADREAM3D_SCENE_H
