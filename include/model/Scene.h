//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_SCENE_H
#define MIRADREAM3D_SCENE_H
#include <map>

#include "Camera.h"
#include "Mesh.h"
#include "texture_types.hpp"

class Node;

const Material defaultMaterial {
    .name="defaultmaterial",
    .ColorTextureID = 0 ,
    .Ka        = {0.2f, 0.2f, 0.2f},
    .Kd        = {0.8f, 0.8f, 0.8f},
    .Ks        = {0.5f, 0.5f, 0.5f},
     // sera remplacé par ta texture de test
    .alpha     = 1.0f,
    .shininess        = 32.0f


};

class Scene
{
public:
    Scene();
    ~Scene();
    Node* getRootNode() const { return mRootNode; }
    const std::vector<Mesh> &getMeshes() const;
    void clearScene();
    void addNode(Node* nodePtr) const;
    void addMesh(const Mesh &mesh);
    /**
     * @brief
     * @param path
     * @return New texture ID, old texture ID if already exists, -1 if import failed
     */
    int32_t loadQTImageAsTexture(const QString &path);
    void addMaterial(const Material &material);
    Mesh *newMesh();
    void removeLastMesh();

    Material* giveNewMaterial(std::string &name);
    uint32_t getMaterialID(std::string &name) ;
    const Material* getMaterial(uint32_t id) const {return &mMaterialList[id];}
private:
    Camera mSceneCamera; //TODO: plusieurs caméras
    glm::vec3 mLight; //TODO: plusieurs lumières
    Node *mRootNode = nullptr;
    std::vector<Mesh> mMeshList;
    std::vector<Texture> mTextureList;
    std::map<std::string, uint32_t> mTextureNames;
    std::map<std::string, uint32_t> mMaterialNames;

    std::vector<Material> mMaterialList;
};

#endif //MIRADREAM3D_SCENE_H
