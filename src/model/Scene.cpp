//
// Created by jassoka on 6/1/26.
//
#include "model/Scene.h"
#include "model/Node.h"
#include "glm/trigonometric.hpp"

static constexpr glm::vec3 defaultSceneCameraUp {1, 1, 1}; //TODO: mettre des vraies valeurs
static constexpr glm::vec3 defaultSceneCameraRight {1, 1, 1};
static constexpr glm::vec3 defaultSceneCameraLookAt {1, 1, 1};
static constexpr glm::vec3 defaultSceneCameraPosition {1, 1, 1};
static constexpr float defaultSceneCameraFOV = glm::radians(45.0f);
static constexpr float defaultSceneCameraNearPlane = 0.1f;
static constexpr float defaultSceneCameraFarPlane = 100.0f;
static constexpr float defaultSceneAspectRatio = 16.0f/9.0f;

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
    delete mRootNode;
}

void Scene::clearScene()
{
    mMeshList.clear();
    //TODO vider node (ils n'existent pas)
}

void Scene::addNode(Node* nodePtr) const
{
    dynamic_cast<HierarchyNode*>(this->mRootNode)->addChild(nodePtr);
}
void Scene::addMesh(const Mesh &mesh) {
    this->mMeshList.push_back(mesh);
}
/*
void Scene::addTexture(const Texture &texture) {
    this->mTextureList.push_back(texture);
}*/
void Scene::addMaterial(const Material &material) {
    this->mMaterialList.push_back(material);
}

const std::vector<Mesh> &Scene::getMeshes() const
{
    return mMeshList;
}

Mesh *Scene::newMesh() {
    addMesh(Mesh(0));
    return &mMeshList[mMeshList.size()-1];
}
void Scene::removeLastMesh() {
    mMeshList.pop_back();
}

int32_t Scene::loadQTImageAsTexture(const QString &path)
{
    const auto strPath = path.toStdString();
    if (const auto it = mTextureNames.find(strPath); it != mTextureNames.end()) // texture already exists
    {
        return it->second;
    }
    QImage image(path);
    if (image.isNull()) return -1;
    image = image.convertToFormat(QImage::Format_RGBA8888);
    assert(image.width() == image.height() == TEXTURE_SIZE); //TODO un peu violent

    const uint32_t textureID = mTextureList.size();
    mTextureList.push_back(Texture(image.constBits()));
    mTextureNames[strPath] = textureID;
    return textureID;
}