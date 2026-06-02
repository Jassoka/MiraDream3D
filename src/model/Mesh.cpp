#include "model/Mesh.h"

#include <iostream>
#include <assimp/vector3.h>

Mesh::Mesh(uint32_t MaterialID):mMaterialID(MaterialID) {
}
Mesh::Mesh(aiMesh &meshAi) {
    this->mMaterialID=meshAi.mMaterialIndex;

    for (uint32_t i=0;i<meshAi.mNumVertices;i++) {
        aiVector3D pos = meshAi.mVertices[i];
        //aiVector3D normale = meshAi.mNormals[i];
        //aiVector3D uv = meshAi.mTextureCoords[0][i];
        this->mVertices.push_back(
            Vertex {
                pos[0],pos[1],pos[2],
                //normale[0],normale[1],normale[2],
                //uv[0],uv[1]
                0,0,0,0,0
            }
        );
    }

    for (uint32_t i=0;i<meshAi.mNumFaces;i++) {
        aiFace* faceAi = &meshAi.mFaces[i];
        this->mFaces.push_back(std::vector<uint32_t>());
        for (uint32_t k=0;k<faceAi->mNumIndices;k++) {
            this->mFaces[i].push_back(faceAi->mIndices[k]);
        }

    }
    printMesh();
}


void Mesh::printMesh() {
    for (auto vertex : mVertices) {

        std::cout << vertex.x <<" "<< vertex.y <<" "<< vertex.z<<std::endl;
    }
    std::cout << "faces" <<std::endl;
    for (auto face : this->mFaces) {

        for (auto vertex : face) {
            std::cout << vertex << " ";
        }
        std::cout  <<std::endl;
    }
}


void Mesh::addVertex(Vertex &vertex) {
    this->mVertices.push_back(vertex);
}
void Mesh::addEdge(Edge &edge) {
    this->mEdges.push_back(edge);
}
void Mesh::addHalfEdge(HalfEdge &halfEdge) {
    this->mHalfEdges.push_back(halfEdge);
}
void Mesh::addFace(Face &face) {
    this->mFaces.push_back(face);
}