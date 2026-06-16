//
// Created by jassoka on 6/1/26.
//
#include "model/Scene.h"

Scene::Scene():
    mSceneCamera(Camera (
            defaultSceneCameraUp,
            defaultSceneCameraRight,
            defaultSceneCameraLookAt,
            defaultSceneCameraPosition,
            defaultSceneCameraFOV,
            defaultSceneCameraNearPlane,
            defaultSceneCameraFarPlane,
            defaultSceneAspectRatio
        ))
{
    std::string name="Root node";
    mRootNode =static_cast<Node*>(new HierarchyNode(name));
}

Scene::~Scene() {
    delete static_cast<Node*>(mRootNode);
}

void Scene::addNode(Node* nodePtr) {
    if (mRootNode) {
        static_cast<HierarchyNode *>(mRootNode)->addChild(nodePtr);
    }
    else mRootNode=nodePtr;
}
void Scene::addMesh(Mesh &mesh) {
    this->mMeshList.push_back(mesh);
}

Mesh* Scene::newMesh() {
    auto m = Mesh(0);
    addMesh(m);
    return &mMeshList[mMeshList.size()-1];
}
void Scene::removeLastMesh() {
    mMeshList.pop_back();
}
void Scene::addTexture(Texture &texture) {
    this->mTextureList.push_back(texture);
}
void Scene::addMaterial(Material &material) {
    this->mMaterialList.push_back(material);
}

const std::vector<Mesh> &Scene::getMeshes() const
{
    return mMeshList;
}

