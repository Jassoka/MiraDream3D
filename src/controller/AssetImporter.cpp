
#include <iostream>
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include "model/Scene.h"
#include "model/geometry.hpp"
#include "model/Mesh.h"
#include "model/Node.h"
#include "controller/AssetImporter.h"


/**
 * @brief //TODO
 * @param nodeAi
 * @param sceneMeshes
 * @return
 */
Node* aiNodeToNode(aiNode* nodeAi,Mesh** sceneMeshes);

AssetImporter::AssetImporter(Scene* scene)
{
    mScene = scene;
}

bool AssetImporter::loadAssimpScene(const std::string &path) const {
    //Imporation
    Assimp::Importer importer;

    const aiScene* aiScene = importer.ReadFile(path,
        aiProcess_JoinIdenticalVertices|
        aiProcess_GenSmoothNormals//aiProcess_Triangulate
    //TODO bah la on denature le fichier pardis, implementer la gestion de plusieurs nor;ales par point
    );

    if (!aiScene) {
        return false;
    }

    //On convertit tous les meshes, on regardera dans ce tableau pour creer les nodes
    Mesh* sceneMeshes [aiScene->mNumMeshes];

    for (uint32_t i=0;i< aiScene->mNumMeshes;i++) {
        auto mesh = Mesh(*aiScene->mMeshes[i]);
        mScene->addMesh(mesh);
    }

    //On convertit recursivement les nodes et on les ajoute a la scene
    mScene->addNode(aiNodeToNode(aiScene->mRootNode,sceneMeshes));
    return true;
}



Node* aiNodeToNode(aiNode* nodeAi, Mesh** sceneMeshes){
    Node* nodePtr = new Node();
    //On ajoute les meshes
    for (uint32_t i=0;i< nodeAi->mNumMeshes;i++) {
        nodePtr->addMesh(nodeAi->mMeshes[i]);
    }
    //On cree les enfants
    for (uint32_t i=0;i< nodeAi->mNumChildren;i++) {
        nodePtr->addChild(aiNodeToNode(nodeAi->mChildren[i],sceneMeshes));
    }

    return(nodePtr);
}

