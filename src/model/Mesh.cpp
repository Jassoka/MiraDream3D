#include "model/Mesh.h"

#include <iostream>
#include <unordered_map>

#include "glm/geometric.hpp"
#include "model/MeshBuilder.h"


Mesh::Mesh(const uint32_t materialID,const std::vector<glm::vec3> &positions,const std::vector<SizedFace> &faces, const glm::mat4 &translationRotationMatrix, const float scale):
mMaterialID(materialID)
{
    MeshBuildData data;
    data.positions = positions;
    data.faces = faces;
    MeshBuilder::build(this, data);
}

#ifdef ENABLE_DEBUG
bool Mesh::operator==(const Mesh& other) const
{
    const bool sameVertices = mRenderVertices == other.mRenderVertices;
    const bool sameFaces = mRenderFaces == other.mRenderFaces;
    return sameFaces && sameVertices; //TODO: same edges and half edges
}

std::ostream& operator<<(std::ostream& os, const Mesh &mesh) {
    os << "Vertices:" << '\n' << "{ ";
    for (const auto vertex : mesh.getRenderVertices()) {
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
#endif

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

void Mesh::swapFaceOrientation(const uint32_t faceID)
{
    Face &rf = mRenderFaces[faceID];
    Face &gf = mGeometricFaces[faceID];
    const uint32_t n = mVertexCountPerFace[faceID];
    for (uint32_t i = 0; i < n/2; i++)
    {
        const uint32_t tmp = rf[i];
        rf[i] = rf[n-i-1];
        rf[n-i-1] = tmp;

        const uint32_t tmp_ = gf[i];
        gf[i] = gf[n-i-1];
        gf[n-i-1] = tmp_;
    }
}

int Mesh::getNextIndice(const uint32_t faceID, const uint32_t vertexID) const
{
    const int n = getNbVertex(faceID);
    for (int i = 0; i < n; i++)
    {
        if (mRenderFaces[faceID][i] == vertexID)
        {
            if (i+1 == n) return 0;
            return i+1;
        }
    }
    return -1;
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
