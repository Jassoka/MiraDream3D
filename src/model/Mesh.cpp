#include "model/Mesh.h"

#include <iostream>
#include <unordered_map>
#include <assimp/include/assimp/mesh.h>
#include <assimp/vector3.h>
#include <sys/stat.h>

#include "glm/geometric.hpp"
#include "model/HalfEdgeBuilder.h"

halfEdgeDirection Mesh::defaultHalfEdgeDirection = ABC;

Mesh::Mesh(uint32_t MaterialID):mMaterialID(MaterialID) {

}
/*
Mesh::Mesh(aiMesh &meshAi) {
    this->mMaterialID=meshAi.mMaterialIndex;


    bool hasNormales =meshAi.HasNormals();
    bool hasUV =meshAi.HasTextureCoords(0);

    for (uint32_t i=0;i<meshAi.mNumVertices;i++) {
        aiVector3D pos = meshAi.mVertices[i];
        aiVector3D normale = hasNormales ? meshAi.mNormals[i]:aiVector3D(0.0,0.0,0.0);//TODO ca explose certainement si pas de normales
        aiVector3D uv = hasUV ? meshAi.mTextureCoords[0][i]:aiVector3D(0.0,0.0,0.0);
        this->mVertices.push_back(
            Vertex {
                pos[0],pos[1],pos[2],
                normale[0],normale[1],normale[2],
                uv[0],uv[1]
            }
        );
    }

    for (uint32_t i=0;i<meshAi.mNumFaces;i++) {
        const aiFace* faceAi = &meshAi.mFaces[i];
        if (faceAi->mNumIndices > 4) exit(1); //TODO le handle pour de vrai
        if (faceAi->mNumIndices == 4)
        {
            mFaces.push_back({faceAi->mIndices[0], faceAi->mIndices[1], faceAi->mIndices[2], faceAi->mIndices[3]});
        }
        else if (faceAi->mNumIndices == 3)
        {
            mFaces.push_back({faceAi->mIndices[0], faceAi->mIndices[1], faceAi->mIndices[2], 0});
        }
        mVertexCountPerFace.push_back(faceAi->mNumIndices);
    }
    triangulate();
    generateHalfEdges();
}*/

bool Mesh::operator==(const Mesh& other) const
{
    const bool sameVertices = mVertices == other.mVertices;
    const bool sameFaces = mRenderFaces == other.mRenderFaces;
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
    mVertices.push_back(vertex);
}
void Mesh::addGeometricVertex(const GeometricVertex &vertex) {
    mGeometricVertices.push_back(vertex);
}
void Mesh::addEdge(const Edge &edge) {
    mEdges.push_back(edge);
}
void Mesh::addHalfEdge(const HalfEdge &halfEdge)
{
    mHalfEdges.push_back(halfEdge);
}
void Mesh::addQuad(const Face &geomFace,const Face &renderFace) {
    mVertexCountPerFace.push_back(4);
    mRenderFaces.push_back(renderFace);
    mGeometricFaces.push_back(geomFace);
}

void Mesh::addTriangle(const Face &geomFace,const Face &renderFace) {
    mVertexCountPerFace.push_back(3);
    mRenderFaces.push_back(renderFace);
    mGeometricFaces.push_back(geomFace);
}

/*
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
*/

void Mesh::generateHalfEdges(const std::vector<std::vector<uint32_t>> *facesPerVertex)
{
    HalfEdgeBuilder::build(*this, facesPerVertex);
    /*
#ifdef TEST_HALFEDGES
    std::cout << "Nb of half edges: " << mHalfEdges.size() << '\n';
    std::cout << "Nb of edges: "<< mEdges.size() << '\n';
    for (auto v : normalPerFace) {std::cout << v.x << " " << v.y << " " << v.z << '\n';}
#endif */
}

void Mesh::triangulate()
{
    mTriangles.clear();
    for (int i = 0; i < mRenderFaces.size(); i++)
    {
        Face &f = mRenderFaces[i];
        if (mVertexCountPerFace[i] == 3)
        {
            mTriangles.push_back(Triangle {f[0], f[1], f[2]});
        }
        else
        {

            glm::vec3 A=mVertices[f[0]].toVec3();
            glm::vec3 B=mVertices[f[1]].toVec3();
            glm::vec3 C=mVertices[f[2]].toVec3();
            glm::vec3 D=mVertices[f[3]].toVec3();
            glm::vec3 mid = A+C;
            mid/=2;
            //la diagonale AC sort du quad, on divise donc selon BD
            //Car si mid etait dans le carre, B et D seraient dans la meme direction et sens opposés
            if (glm::dot(mid-B,mid-D)>0) {
                mTriangles.push_back(Triangle {f[0], f[1], f[3]});
                mTriangles.push_back(Triangle {f[1], f[2], f[3]});
            }
            else {
                mTriangles.push_back(Triangle {f[0], f[1], f[2]});
                mTriangles.push_back(Triangle {f[0], f[2], f[3]});
            }

            //DONE MARCHE meme SI pas CONVEXE pourtant c'est LA MEILLEURE PROPRIÉTÉ DU MONDE !!!!!!!1§§1§§
        }
    }
#ifdef ENABLE_DEBUG
    std::cout << *this << std::endl;
#endif
}

halfEdgeDirection Mesh::findFaceOrientation(uint32_t AId,const std::vector<uint32_t> &adjacentFaces, glm::vec3 *normalPtr) const
{
    //Si point isolé
    glm::vec3 normal;
    if  (adjacentFaces.size()==0) {
        normal=glm::vec3(0.0,0.0,1.0);
        if (normalPtr) *normalPtr = normal;
        return ABC;
    }
    //Calcul de E
    glm::vec3 E=glm::vec3(0.0); // E : mean of points adjacents to A
    std::vector<bool> visitedVertices(mVertices.size(),false);
    visitedVertices[AId]=true;
    uint32_t nAdjacentVertices=0;
    for (auto faceId : adjacentFaces) {
        auto face=mRenderFaces[faceId];
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
    normal=getNormal(mRenderFaces[adjacentFaces[0]],halfEdgeDirection::ABC);

    halfEdgeDirection orientation;
    if (glm::dot(EA,normal)>0) {
        orientation = ABC;
    }
    else {
        normal=-normal;
        orientation = ACB;
    }
    if (normalPtr) *normalPtr = normal;
    return orientation;
}



glm::vec3 Mesh::getNormal(const Face &face, const halfEdgeDirection orientation) const
{
    Vertex AVertex=mVertices[face[0]];
    Vertex BVertex=mVertices[face[1]];
    Vertex CVertex=mVertices[face[2]];
    if (orientation==ACB) {
        std::swap(BVertex,CVertex);
    }

    const glm::vec3 A=glm::vec3(AVertex.x,AVertex.y,AVertex.z);
    const glm::vec3 B=glm::vec3(BVertex.x,BVertex.y,BVertex.z);
    const glm::vec3 C=glm::vec3(CVertex.x,CVertex.y,CVertex.z);
        //AB^AC normalisé
    return(glm::normalize(glm::cross(B-A,C-A)));
}
