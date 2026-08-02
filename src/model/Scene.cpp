//
// Created by jassoka on 6/1/26.
//
#include "model/Scene.h"
#include "model/Node.h"
#include "glm/trigonometric.hpp"
#include "model/SceneImport.h"

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
    const std::string name="Root node";
    mRootNode =static_cast<Node*>(new HierarchyNode(name));
    auto defaultTextureData = std::array<uint8_t, TEXTURE_SIZE*TEXTURE_SIZE*4>();
    defaultTextureData.fill(255);
    //loadQTImageAsTexture(":/assets/default_texture.png");
    mTextureList.emplace_back(defaultTextureData.data());
    //TODO petit probleme on peut magouiller pour modifier le materiau par défaut la
    mMaterialRegistry.newMaterial("");
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
    dynamic_cast<HierarchyNode*>(this->mRootNode)->pushChild(nodePtr);
}
void Scene::addMesh(const Mesh &mesh) {
    this->mMeshList.push_back(mesh);
}
/*
void Scene::addTexture(const Texture &texture) {
    this->mTextureList.push_back(texture);
}*/


int32_t Scene::getTextureId(const std::string & path) {
    if (mTexturePaths.find(path)== mTexturePaths.end()) {
        return loadQTImageAsTexture(QString::fromStdString(path));
    }
    return mTexturePaths[path];
}


const std::vector<Mesh> &Scene::getMeshes() const
{
    return mMeshList;
}

Mesh *Scene::newMesh() {
    addMesh(Mesh());
    return &mMeshList[mMeshList.size()-1];
}
void Scene::removeLastMesh() { //TODO memory leak je crois
    mMeshList.pop_back();
}

int32_t Scene::loadQTImageAsTexture(const QString &path)
{
    const auto strPath = path.toStdString();
    if (const auto it = mTexturePaths.find(strPath); it != mTexturePaths.end()) // texture already exists
    {
        return it->second;
    }
    QImage image(path);
    if (image.isNull()) return -1;
    const int width = image.width();
    const int height = image.height();
    if (width != height) return -1;

    if (width != TEXTURE_SIZE)
    {
        image = image.scaled(TEXTURE_SIZE, TEXTURE_SIZE ,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    image = image.convertToFormat(QImage::Format_RGBA8888);
    image = image.mirrored(false, true);

    const uint32_t textureID = mTextureList.size();
    mTextureList.push_back(Texture(image.constBits()));
    mTexturePaths[strPath] = textureID;
    return textureID;
}


void Scene::appendImport(SceneImport& import)
{
    auto &importMeshes = import.mLocalMeshes;
    auto &sceneMeshes = mMeshList;

    //std::ostringstream warnings;

    // Generating maps
    std::unordered_map<uint32_t, uint32_t> localToGlobalTextureMap;
    std::unordered_map<uint32_t, uint32_t> localToGlobalMaterialMap;
    for (const auto &[path, localId] : import.mTexturePaths)
    {
        const int32_t globalId = getTextureId(path);
        if (globalId < 0)
        {
           // warnings << "file not found: " << path << '\n';
            localToGlobalTextureMap[localId] = DEFAULT_TEXTURE;
        }
        else
            localToGlobalTextureMap[localId] = globalId;
    }

    for (const auto &[name, localId] : import.mLocalMaterialRegistry.materialNames)
    {
        Material &material = import.mLocalMaterialRegistry.getMaterial(localId);
        localToGlobalMaterialMap[localId] = createNewMaterial(name, material);
        // remapping texture to correct index
        const auto globalId = localToGlobalMaterialMap[localId];
        const auto localTextureId = material.ColorTextureID;
        // Si c'est la texture par défaut, elle ne change pas
        const auto globalTextureId =
            localTextureId == DEFAULT_TEXTURE ? DEFAULT_TEXTURE : localToGlobalTextureMap[localTextureId];

        mMaterialRegistry.getMaterial(globalId).ColorTextureID = globalTextureId;
    }

    for (auto &mesh: importMeshes)
    {
        auto &meshMaterials = mesh.getMaterialList();
        for (int i = 0; i < meshMaterials.size(); i++) { meshMaterials[i] = localToGlobalMaterialMap[meshMaterials[i]]; }
    }

    sceneMeshes.reserve(sceneMeshes.size()+importMeshes.size());
    sceneMeshes.insert(sceneMeshes.end(), importMeshes.begin(), importMeshes.end());

    /*
    const auto warningsStr = warnings.str();
    if (!warningsStr.empty())
        ErrorController::showWarnings(warningsStr);*/
}