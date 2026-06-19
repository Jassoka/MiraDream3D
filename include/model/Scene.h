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
private:
    Camera mSceneCamera; //TODO: plusieurs caméras
    glm::vec3 mLight; //TODO: plusieurs lumières
    Node *mRootNode = nullptr;
    std::vector<Mesh> mMeshList;
    std::vector<Texture> mTextureList;
    std::map<std::string, uint32_t> mTextureNames;
    std::vector<Material> mMaterialList;
};

#endif //MIRADREAM3D_SCENE_H
