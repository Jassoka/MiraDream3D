
#include <iostream>
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include "model/Scene.h"
#include "model/geometry.hpp"
#include "model/Mesh.h"
#include "model/Node.h"


Node* aiNodeToNode(aiNode* nodeAi,Mesh** sceneMeshes);


void loadFile(std::string file, Scene scene) {
    //Imporation
    Assimp::Importer importer;

    const aiScene* aiScene = importer.ReadFile(file,
        0//aiProcess_Triangulate

    );

    if (!aiScene) {
        std::cerr << importer.GetErrorString() << std::endl;
    }
    assert(aiScene!=nullptr);
    //


    //On convertit tous les meshes, on regardera dans ce tableau pour creer les nodes
    Mesh* sceneMeshes [aiScene->mNumMeshes];

    for (uint32_t i=0;i< aiScene->mNumMeshes;i++) {
        Mesh mesh = Mesh(*aiScene->mMeshes[i]);
        scene.addMesh(mesh);
    }

    //On convertit recursivement les nodes et on les ajoute a la scene
    scene.addNode(aiNodeToNode(aiScene->mRootNode,sceneMeshes));
}



Node* aiNodeToNode(aiNode* nodeAi,Mesh** sceneMeshes){
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

