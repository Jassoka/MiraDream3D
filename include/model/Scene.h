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

constexpr Material defaultMaterial {
    .ColorTextureID = DEFAULT_TEXTURE,
    .Ka        = {1.0f, 1.0f, 1.0f},
    .Kd        = {1.0f, 1.0f, 1.0f},
    .Ks        = {0.05f, 0.05f, 0.05f},
     // sera remplacé par ta texture de test
    .alpha     = 1.0f,
    .shininess        = 32.0f
};

/**
 * @brief Class for the main scene
 * The scene owns all meshes, materials and textures
 */
class Scene
{
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

    /**
     * @brief Creates a material in the scene with default configuration
     * @param name Name of material
     * @return Pointer to material
     */
    Material* createNewMaterial(const std::string &name);
    /**
     * @brief Returns ID to material called name
     * @warning Does not check if material exists //TODO en vrai ce serait bien
     */
    uint32_t getMaterialID(const std::string &name) ;
    /**
     * @brief Returns pointer to material indexed id
     * @warning Does not check if material exists
     */
    const Material* getMaterial(const uint32_t id) const {return &mMaterialList[id];}

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
    /** @brief Map associating a name to each Texture ID */
    std::map<std::string, uint32_t> mTextureNames;
    /** @brief List of materials in scene */
    std::vector<Material> mMaterialList;
    /** @brief Map associating a name to each Material ID */
    std::map<std::string, uint32_t> mMaterialNames;

};

#endif //MIRADREAM3D_SCENE_H
