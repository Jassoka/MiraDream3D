//
// Created by Mathis Pean on 27/05/2026.
//

#ifndef MIRADREAM3D_SCENE_HPP
#define MIRADREAM3D_SCENE_HPP
#include "Camera.h"
#include "Node.h"

class Scene
{
    Camera mCamera; //TODO: plusieurs caméras
    glm::vec3 mLight; //TODO: plusieurs lumières
    Node *mRootNode = nullptr;
    std::vector<Texture> mTextureList;
    std::vector<Material> mMaterialList;
};

#endif //MIRADREAM3D_SCENE_HPP
