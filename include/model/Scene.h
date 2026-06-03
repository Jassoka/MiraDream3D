//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_SCENE_H
#define MIRADREAM3D_SCENE_H
#include "Camera.h"
#include "Node.h"
#include "texture_types.hpp"

class Scene
{
private:
    static constexpr glm::vec3 defaultSceneCameraUp {1, 1, 1}; //TODO: mettre des vraies valeurs
    static constexpr glm::vec3 defaultSceneCameraRight {1, 1, 1};
    static constexpr glm::vec3 defaultSceneCameraLookAt {1, 1, 1};
    static constexpr glm::vec3 defaultSceneCameraPosition {1, 1, 1};
    static constexpr float defaultSceneCameraFOV = glm::radians(45.0f);
    static constexpr float defaultSceneCameraNearPlane = 0.1f;
    static constexpr float defaultSceneCameraFarPlane = 100.0f;
    static constexpr float defaultSceneAspectRatio = 16.0f/9.0f;
    Camera mSceneCamera; //TODO: plusieurs caméras
    glm::vec3 mLight; //TODO: plusieurs lumières
    Node *mRootNode = nullptr;
    std::vector<Mesh> mMeshList;
    std::vector<Texture> mTextureList;
    std::vector<Material> mMaterialList;
public:

    void addNode(Node* nodePtr);
    void addMesh(Mesh &mesh);
    void addTexture(Texture &texture);
    void addMaterial(Material &material);
    const std::vector<Mesh>& getMeshes() const;
    Scene();
    ~Scene();
};

#endif //MIRADREAM3D_SCENE_H
