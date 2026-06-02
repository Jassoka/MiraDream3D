//
// Created by jassoka on 6/1/26.
//
#import "model/Scene.h"

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
        )),
    mRootNode(new Node())
{
}

Scene::~Scene() {
    delete mRootNode;
}

void Scene::addNode(Node* nodePtr) {
    this->mRootNode->addChild(nodePtr);
}
void Scene::addMesh(Mesh &mesh) {
    this->mMeshList.push_back(mesh);
}
void Scene::addTexture(Texture &texture) {
    this->mTextureList.push_back(texture);
}
void Scene::addMaterial(Material &material) {
    this->mMaterialList.push_back(material);
}