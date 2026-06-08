#include "model/Mesh.h"

#include <iostream>
#include <unordered_map>
#include <assimp/include/assimp/mesh.h>
#include <assimp/vector3.h>

#include "glm/geometric.hpp"

Mesh::Mesh(uint32_t MaterialID):mMaterialID(MaterialID) {
}
Mesh::Mesh(aiMesh &meshAi) {
    this->mMaterialID=meshAi.mMaterialIndex;

    for (uint32_t i=0;i<meshAi.mNumVertices;i++) {
        aiVector3D pos = meshAi.mVertices[i];
        aiVector3D normale = meshAi.mNormals[i];
        //aiVector3D uv = meshAi.mTextureCoords[0][i];
        this->mVertices.push_back(
            Vertex {
                pos[0],pos[1],pos[2],
                normale[0],normale[1],normale[2],
                //uv[0],uv[1],
            }
        );
    }

    for (uint32_t i=0;i<meshAi.mNumFaces;i++) {
        const aiFace* faceAi = &meshAi.mFaces[i];
        if (faceAi->mNumIndices > 4) exit(1); //TODO le handle pour de vrai
        if (faceAi->mNumIndices == 4)
        {
            this->mFaces.push_back({faceAi->mIndices[0], faceAi->mIndices[1], faceAi->mIndices[2], faceAi->mIndices[3]});
        }
        else if (faceAi->mNumIndices == 3)
        {
            this->mFaces.push_back({faceAi->mIndices[0], faceAi->mIndices[1], faceAi->mIndices[2], 0});
        }
        this->mVertexCountPerFace.push_back(faceAi->mNumIndices);
    }
    triangulate();
    generateEdges();
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
    mEdges.clear(); //TODO pue la merde
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


void Mesh::generateHalfEdges()
{
    generateEdges();
    // On suppose le mesh un manifold
    // Trouver la liste des extremums en x
    float xMax = mVertices[0].x;
    for (uint32_t idxV = 1; idxV < mVertices.size(); idxV ++)
    {
        if (mVertices[idxV].x > xMax) xMax = mVertices[idxV].x;
    }
    std::unordered_set<uint32_t> extremums;
    for (uint32_t idxV = 0; idxV < mVertices.size(); idxV ++)
    {
        if (xMax - mVertices[idxV].x < FLT_EPSILON) extremums.insert(idxV);
    }
    // On cherche un point Vx tq il existe un voisin non extremum
    uint32_t Vx = -1;
    for (auto [origin, end] : mEdges)
    {
        if (extremums.find(origin) == extremums.end() && extremums.find(end) != extremums.end())
        {
            Vx = end;
            break;
        }
        if (extremums.find(origin) != extremums.end() && extremums.find(end) == extremums.end())
        {
            Vx = origin;
            break;
        }
    }
    // Si il n'existe pas: alors c'est plat sur x, on prend n'importe quelle premiere normale
    // Sinon: soit Vx ce point, on nomme E la moyenne des points adjacents
    // On prend n'importe quelle face, et on a EE' (E' proj ortho de E sur la face) sa normale
    // Si E==E' alors n'importe quelle normale

    // Prendre la face en question: A, B, C ...
    // Alors si AB^AC de même sens à n, c'est ABC, sinon CBA

    // Propagation des half edges, jusqu'a que chaque face soit touchée
    /*
    std::vector<HalfEdge> tmpHalfEdges;
    uint32_t halfEdgeCount = 0;
    for (uint32_t fID = 0; fID < mFaces.size(); fID++)
    {
        Face &f = mFaces[fID];
        for (uint32_t vID = 0; vID < getNbVertex(fID); vID++)
        {
            tmpHalfEdges[halfEdgeCount++ ] = HalfEdge{};
        }
        mHalfEdgeIndexPerVertex[vID] = halfEdgeIdx;
        for (auto faces : facesPerVertex[vID])
        {

        }*/
    }
    /*
    auto facesPerVertex = std::vector<std::vector<uint32_t>>(mVertices.size());
    for (uint32_t fID = 0; fID < mFaces.size(); fID++)
    {
        Face &f = mFaces[fID];
        facesPerVertex[f[0]].push_back(fID);
        facesPerVertex[f[1]].push_back(fID);
        facesPerVertex[f[2]].push_back(fID);
        if (isQuad(fID))
        {
            facesPerVertex[f[3]].push_back(fID);
        }
    }
    mHalfEdgeIndexPerVertex.resize(mVertices.size());
    */
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

/**
 *
 * A :
 * E : mean of points adjacents to A
 **/
void Mesh::findNormalAndOrientation(uint32_t AId,std::vector<uint32_t> adjacentFaces,glm::vec3* normalPtr,halfEdgeDirection* directionPtr) {


    //Si point isolé
    if  (adjacentFaces.size()==0) {
        *normalPtr=glm::vec3(0.0,0.0,1.0);
        *directionPtr=ABC;
    }
    //Calcul de E
    glm::vec3 E=glm::vec3(0.0);
    std::vector<bool> visitedVertices(mVertices.size(),false);
    visitedVertices[AId]=true;
    uint32_t nAdjacentVertices=0;
    for (auto faceId : adjacentFaces) {
        auto face=mFaces[faceId];
        for (uint32_t i=0;i<mVertexCountPerFace[faceId];i++) {
            auto vertexId=face[i];
            //on regarde si le sommet a deja ete ajoute a la somme, si non, on le fait et on l'ajoute a la liste des points visites
            if (!visitedVertices[vertexId]){
                auto vertex=mVertices[vertexId];
                E+=glm::vec3(vertex.x,vertex.y,vertex.z);
                visitedVertices[vertexId]=true;
                nAdjacentVertices+=1;
            }
        }
    }

    E/=nAdjacentVertices;
    //E est à présent calculé
    Vertex AVertex=mVertices[AId];
    glm::vec3 A=glm::vec3(AVertex.x,AVertex.y,AVertex.z);
    glm::vec3 EA=A-E;
    glm::vec3 normal=getNormal(mFaces[adjacentFaces[0]],halfEdgeDirection::ABC);
    if (glm::dot(EA,normal)>0) {
        *normalPtr=normal;
        *directionPtr=ABC;
    }
    else {
        *normalPtr=-normal;
        *directionPtr=ACB;
    }
}



glm::vec3 Mesh::getNormal(Face &face,halfEdgeDirection orientation){
    Vertex AVertex=mVertices[face[0]];
    Vertex BVertex=mVertices[face[1]];
    Vertex CVertex=mVertices[face[2]];
    if (orientation==ACB) {
        std::swap(BVertex,CVertex);
    }

    glm::vec3 A=glm::vec3(AVertex.x,AVertex.y,AVertex.z);
    glm::vec3 B=glm::vec3(BVertex.x,BVertex.y,BVertex.z);
    glm::vec3 C=glm::vec3(CVertex.x,CVertex.y,CVertex.z);
        //AB^AC normalisé
    return(glm::normalize(glm::cross(B-A,C-A)));
}