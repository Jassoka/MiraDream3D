//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_SCENE_H
#define MIRADREAM3D_SCENE_H
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
    void addTexture(const Texture &texture);
    void addMaterial(const Material &material);
    Mesh *newMesh();
    void removeLastMesh();
private:
    Camera mSceneCamera; //TODO: plusieurs caméras
    glm::vec3 mLight; //TODO: plusieurs lumières
    Node *mRootNode = nullptr;
    std::vector<Mesh> mMeshList;
    std::vector<Texture> mTextureList;
    std::vector<Material> mMaterialList;
};

#endif //MIRADREAM3D_SCENE_H
