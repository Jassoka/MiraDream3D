#include <iostream>
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include "model/Scene.h"
#include "model/Node.h"
#include "model/AssetImporter.h"

#include "objParsing.hpp"


/**
 * @brief //TODO
 * @param nodeAi
 * @param sceneMeshes
 * @return
 */
Node* aiNodeToNode(aiNode* nodeAi,Mesh** sceneMeshes);


bool AssetImporter::loadAssimpScene(const std::string &path, Scene *scene) {
    //Imporation
    Assimp::Importer importer;

    const aiScene* aiScene = importer.ReadFile(path, //0
        aiProcess_JoinIdenticalVertices//|
        //aiProcess_GenSmoothNormals//aiProcess_Triangulate
    //TODO bah la on denature le fichier pardis, implementer la gestion de plusieurs nor;ales par point
    );

    if (!aiScene) {
        return false;
    }

    //On convertit tous les meshes, on regardera dans ce tableau pour creer les nodes
    Mesh* sceneMeshes [aiScene->mNumMeshes];

    for (uint32_t i=0;i< aiScene->mNumMeshes;i++) {
        auto mesh = Mesh(*aiScene->mMeshes[i]);
        scene->addMesh(mesh);
    }


    //On convertit recursivement les nodes et on les ajoute a la scene
    //scene->addNode(aiNodeToNode(aiScene->mRootNode,sceneMeshes));
    return true;
}



Node* aiNodeToNode(aiNode* nodeAi, Mesh** sceneMeshes){
    Node* nodePtr = new Node();
    //On ajoute les meshes
    for (uint32_t i=0;i< nodeAi->mNumMeshes;i++) {
        //static_cast<HierarchyNode*>(nodePtr)->addMesh(nodeAi->mMeshes[i]);
    }
    //On cree les enfants
    for (uint32_t i=0;i< nodeAi->mNumChildren;i++) {
        //static_cast<HierarchyNode*>(nodePtr)->addChild(aiNodeToNode(nodeAi->mChildren[i],sceneMeshes));
    }

    return(nodePtr);
}



bool AssetImporter::loadObjFile(const std::string &path, Scene* scene) {
    ObjParser parser = ObjParser(path,scene);
    parser.parse();
        return true;
}