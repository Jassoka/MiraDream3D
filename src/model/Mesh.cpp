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
    const bool sameVertices = mRenderVertices == other.mRenderVertices;
    const bool sameFaces = mRenderFaces == other.mRenderFaces;
    return sameFaces && sameVertices; //TODO: same edges and half edges
}

std::ostream& operator<<(std::ostream& os, const Mesh &mesh) {
    os << "Vertices:" << '\n' << "{ ";
    for (const auto vertex : mesh.getVertices()) {
        os << "(" << vertex.x <<", "<< vertex.y <<", "<< vertex.z<<") ";
    }
    os << "}" << '\n';
    os << "Render faces: " << '\n' << "{ ";
    for (const auto face : mesh.getRenderFaces()) {

        for (const auto vertex : face) {
            os << vertex << " ";
        }
        os << "| ";
    }
    os << "}" << '\n';
    return os;
}


void Mesh::addVertex(const RenderVertex &vertex) {
    mRenderVertices.push_back(vertex);
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

            glm::vec3 A=mRenderVertices[f[0]].toVec3();
            glm::vec3 B=mRenderVertices[f[1]].toVec3();
            glm::vec3 C=mRenderVertices[f[2]].toVec3();
            glm::vec3 D=mRenderVertices[f[3]].toVec3();
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
    std::vector<bool> visitedVertices(mGeometricVertices.size(),false);
    visitedVertices[AId]=true;
    uint32_t nAdjacentVertices=0;
    for (auto faceId : adjacentFaces) {
        //On prend une face adjacente
        auto face=mGeometricFaces[faceId];
        for (uint32_t i=0;i<mVertexCountPerFace[faceId];i++) {
            //pour chaque vertex adjacent, s'il est deja visite on s'en branle sinon on l'ajoute a la moyenne
            auto vertexId=face[i];
            //on regarde si le sommet a deja ete ajoute a la somme, si non, on le fait et on l'ajoute a la liste des points visites
            if (!visitedVertices[vertexId]){
                auto vertexPos=getGeometricVertexPosition(vertexId)  ;
                E+=glm::vec3(vertexPos.x,vertexPos.y,vertexPos.z);
                visitedVertices[vertexId]=true;
                nAdjacentVertices+=1;
            }
        }
    }

    E/=nAdjacentVertices;
    //E est à présent calculé
    glm::vec3 A= getGeometricVertexPosition(AId);
    glm::vec3 EA=A-E;
    normal=getNormal(mGeometricFaces[adjacentFaces[0]],halfEdgeDirection::ABC);

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



glm::vec3 Mesh::getNormal(const Face &geomFace, const halfEdgeDirection orientation) const
{
    glm::vec3 APos=getGeometricVertexPosition(geomFace[0]);
    glm::vec3 BPos=getGeometricVertexPosition(geomFace[1]);
    glm::vec3 CPos=getGeometricVertexPosition(geomFace[2]);
    if (orientation==ACB) {
        std::swap(BPos,CPos);
    }

    const auto A=glm::vec3(APos.x,APos.y,APos.z);
    const auto B=glm::vec3(BPos.x,BPos.y,BPos.z);
    const auto C=glm::vec3(CPos.x,CPos.y,CPos.z);
        //AB^AC normalisé
    return(glm::normalize(glm::cross(B-A,C-A)));
}
