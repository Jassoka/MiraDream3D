#include "model/Mesh.h"

#include <iostream>
#include <assimp/include/assimp/mesh.h>
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
        const aiFace* faceAi = &meshAi.mFaces[i];
        if (faceAi->mNumIndices > 4) exit(1); //TODO le handle pour de vrai
        if (faceAi->mNumIndices == 4)
        {
            this->mFaces[i] = {faceAi->mIndices[0], faceAi->mIndices[1], faceAi->mIndices[2], faceAi->mIndices[3]};
        }
        else if (faceAi->mNumIndices == 3)
        {
            this->mFaces[i] = {faceAi->mIndices[0], faceAi->mIndices[1], faceAi->mIndices[2]};
        }
    }
    triangulate();
}

bool Mesh::operator==(const Mesh& other) const
{
    const bool sameVertices = this->mVertices == other.mVertices;
    const bool sameFaces = this->mFaces == other.mFaces;
    return sameFaces && sameVertices; //TODO: same edges and half edges
}

std::ostream& operator<<(std::ostream& os, const Mesh &mesh) {
    os << "Vertices:" << '\n' << "{ ";
    for (const auto vertex : mesh.getVertices()) {
        os << "(" << vertex.x <<", "<< vertex.y <<", "<< vertex.z<<") ";
    }
    os << "}" << '\n';
    os << "Faces: " << '\n' << "{ ";
    for (const auto face : mesh.getFaces()) {

        for (const auto vertex : face) {
            os << vertex << " ";
        }
        os << "| ";
    }
    os << "}" << '\n';
    return os;
}


void Mesh::addVertex(const Vertex &vertex) {
    this->mVertices.push_back(vertex);
}

void Mesh::addEdge(const Edge &edge) {
    this->mEdges.push_back(edge);
}
void Mesh::addHalfEdge(const HalfEdge &halfEdge)
{
    this->mHalfEdges.push_back(halfEdge);
}
void Mesh::addQuad(const Face &face) {
    this->mVertexCountPerFace.push_back(4);
    this->mFaces.push_back(face);
}

void Mesh::addTriangle(const Face &face) {
    this->mVertexCountPerFace.push_back(3);
    this->mFaces.push_back(face);
}

void Mesh::generateEdges()
{
    for (int i = 0; i < mFaces.size(); i++)
    {
        Face &f = mFaces[i];
        mEdges.push_back(Edge {f[0], f[1]});
        mEdges.push_back(Edge {f[1], f[2]});
        if (mVertexCountPerFace[i] == 3)
        {
            mEdges.push_back(Edge {f[2], f[0]});
        }
        if (mVertexCountPerFace[i] == 4)
        {
            mEdges.push_back(Edge {f[2], f[3]});
            mEdges.push_back(Edge {f[3], f[0]});
        }
    }
}

void Mesh::triangulate()
{
    for (int i = 0; i < mFaces.size(); i++)
    {
        Face &f = mFaces[i];
        if (mVertexCountPerFace[i] == 3)
        {
            mTriangles.push_back(Triangle {f[0], f[1], f[2]});
        }
        else
        {
            //TODO MARCHE QUE SI CONVEXE LA MEILLEURE PROPRIÉTÉ DU MONDE !!!!!!!1§§1§§
            mTriangles.push_back(Triangle {f[0], f[1], f[2]});
            mTriangles.push_back(Triangle {f[0], f[2], f[3]});
        }
    }
}
