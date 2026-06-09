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
    generateHalfEdges();
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

void Mesh::generateHalfEdges()
{
    //TODO FUSIONNER LES VERTEX DE MEME POINT (vertex géometriques) (ça implique des faces géométriques distinctes aussi)
    //TODO ITERER SUR CHAQUE COMPOSANTE CONNEXE
    std::vector<std::array<int32_t, 2>> adjacentFacesToEdge;
    std::unordered_map<uint64_t, uint32_t> edgeMap;
    [this, &adjacentFacesToEdge, &edgeMap]
    {
        mEdges.clear();
        // On génère les edges normales également
        uint32_t edgeID = mEdges.size();
        for (int32_t fID = 0; fID < mFaces.size(); fID++)
        {
            Face &f = mFaces[fID];
            const uint32_t faceSize = getNbVertex(fID);
            for (uint32_t i = 0; i < faceSize; i++)
            {
                const uint32_t next_j = (i + 1)%faceSize;
                const uint32_t origin = std::min(f[i], f[next_j]);
                const uint32_t end = std::max(f[i], f[next_j]);
                std::cout << "Face " << fID << " Edge ID pair: (" << origin << ", " << end << ")\n";
                uint64_t key = static_cast<uint64_t>(origin) << 32 | end;
                if (auto it = edgeMap.find(key); it == edgeMap.end()) // not in map
                {
                    mEdges.push_back(Edge{origin, end});
                    adjacentFacesToEdge.push_back(std::array<int32_t, 2>());
                    adjacentFacesToEdge[edgeID] = {fID, -1};
                    edgeMap.insert(std::make_pair(key, edgeID++));
                }
                else // in map
                {
                    adjacentFacesToEdge[it->second][1] = fID;
                }
            }
        }
    }();


    mHalfEdges.clear();
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
    auto facesPerVertex = std::vector<std::vector<uint32_t>>(mVertices.size());
    for (uint32_t fID = 0; fID < mFaces.size(); fID++)
    {
        Face &f = mFaces[fID];
        for (uint32_t i = 0; i < getNbVertex(fID); i++) {
            facesPerVertex[f[i]].push_back(fID);
        }
    }

    std::vector<glm::vec3> normalPerFace(mFaces.size());
    uint32_t faceID;

    // Si il n'existe pas: alors c'est plat sur x, on prend n'importe quelle premiere normale
    halfEdgeDirection firstOrientation = ABC;
    if (Vx == -1)
    {
        faceID = 0;
        normalPerFace[faceID] = getNormal(mFaces[faceID], firstOrientation);
    }
    // Sinon: soit Vx ce point, on nomme E la moyenne des points adjacents
    // On prend n'importe quelle face (0), et on a EE' (E' proj ortho de E sur la face) sa normale
    // Si E==E' alors n'importe quelle normale

    // Prendre la face en question: A, B, C ...
    // Alors si AB^AC de même sens à n, c'est ABC, sinon CBA
    else
    {
        faceID = facesPerVertex[Vx][0];
        findNormalAndOrientation(Vx, facesPerVertex[Vx], normalPerFace[faceID], firstOrientation);
    }
    // 1ere face
    Face &currFace = mFaces[faceID];
    uint32_t faceSize = getNbVertex(faceID);

    std::vector<uint32_t> halfEdgesToIterate;
    uint32_t halfEdgeIterationIndex = 0; // attention tres different de halfEdgeIdx ,c'est l'indice de la liste ci-haut, et non pas l'indice courant de mHalfEdges

    const uint32_t halfEdgeIdx = mHalfEdges.size();
    for (uint32_t i = 0; i < faceSize; i++)
    {
        halfEdgesToIterate.push_back(i);
        const uint32_t next_i = (i+1)%faceSize;
        const uint32_t prev_i = (i + faceSize -1)%faceSize;
        const uint32_t endVertex = currFace[(firstOrientation == ABC) ? next_i : prev_i];
        mHalfEdges.push_back(HalfEdge{halfEdgeIdx + next_i, halfEdgeIdx + prev_i, faceID, -1, currFace[i], endVertex});
    }
    // Propagation des half edges, jusqu'a que chaque face soit touchée

    std::vector<uint8_t> visitedFace(mFaces.size(), 0);
    visitedFace[faceID] = 1;

    while (halfEdgeIterationIndex < halfEdgesToIterate.size()) // tant qu'on a des aretes à parcourir
    {
        const uint32_t currHalfEdgeIdx = halfEdgesToIterate[halfEdgeIterationIndex];
        HalfEdge currHalfEdge = mHalfEdges[currHalfEdgeIdx];
        // Trouver la face voisine (il y en a au plus 1 par la propriété du mesh 2-manifold //TODO verifier que c'est bien 2-manifold à un moment
        const uint32_t neighbouringFace = [&adjacentFacesToEdge, &edgeMap, &visitedFace](const uint32_t a, const uint32_t b, const uint32_t face)
        {
            const uint32_t origin = std::min(a, b);
            const uint32_t end = std::max(a, b);
            const auto it = edgeMap.find(static_cast<uint64_t>(origin) << 32 | end);
            const uint32_t edgeID = it->second;
            if (adjacentFacesToEdge[edgeID][0] == face)
            {
                if (visitedFace[adjacentFacesToEdge[edgeID][1]]) return -1;;
                return adjacentFacesToEdge[edgeID][1];
            }
            if (visitedFace[adjacentFacesToEdge[edgeID][0]]) return -1;;
            return adjacentFacesToEdge[edgeID][0];
        }(currHalfEdge.origin, currHalfEdge.end, currHalfEdge.face);
        if (neighbouringFace == -1)
        {
            halfEdgeIterationIndex++;
            continue;
        }
        visitedFace[neighbouringFace] = 1;
        // On propage sur la nouvelle face
        Face &f = mFaces[neighbouringFace];
        halfEdgeDirection faceOrientation = ABC;
        // la nouvelle half edge commence dans l'autre sens
        uint32_t start = currHalfEdge.end;
        uint32_t end = currHalfEdge.origin;


        uint32_t nVertex = getNbVertex(neighbouringFace);

        uint32_t startIndice = 0;
        const Face& face = mFaces[neighbouringFace];

        for (uint32_t j = 0; j < nVertex; j++)
        {
            if (f[j] == start)
            {
                if (f[(j+1) % nVertex] == end) faceOrientation = ABC;
                else faceOrientation = ACB;
                startIndice = j;
            }
        }


        const uint32_t halfEdgeLast = mHalfEdges.size();
        mHalfEdges[currHalfEdgeIdx].twin = halfEdgeLast; // prochain indice en anticipation

        if (faceOrientation == ABC)
        {
            for (uint32_t i = 0; i < nVertex; i++)
            {
                const uint32_t next_i = (i+1)%nVertex;
                const uint32_t prev_i = ((i + nVertex - 1))%nVertex;
                halfEdgesToIterate.push_back(mHalfEdges.size());
                mHalfEdges.push_back(HalfEdge{halfEdgeLast+next_i, halfEdgeLast+prev_i, neighbouringFace, (i==0)?static_cast<int32_t>(currHalfEdgeIdx):-1, f[(startIndice+i)%nVertex], f[(startIndice+next_i)%nVertex]});
            }
        }
        else
        {
            for (uint32_t i = 0; i < nVertex; i++)
            {
                const uint32_t next_i = (i+1)%nVertex;
                const uint32_t prev_i = ((i + nVertex - 1))%nVertex;
                halfEdgesToIterate.push_back(mHalfEdges.size());
                mHalfEdges.push_back(HalfEdge{halfEdgeLast+next_i, halfEdgeLast+prev_i, neighbouringFace, (i==0)?static_cast<int32_t>(currHalfEdgeIdx):-1, f[(startIndice+i)%nVertex], f[(startIndice+prev_i)%nVertex]});
            }
        }
        halfEdgeIterationIndex++;
    }
    std::cout << mEdges.size()<<std::endl;
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
    std::cout << *this << std::endl;
}

void Mesh::findNormalAndOrientation(uint32_t AId,std::vector<uint32_t> &adjacentFaces, glm::vec3 &normal, halfEdgeDirection &direction)
{
    //Si point isolé
    if  (adjacentFaces.size()==0) {
        normal=glm::vec3(0.0,0.0,1.0);
        direction=ABC;
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
    normal=getNormal(mFaces[adjacentFaces[0]],halfEdgeDirection::ABC);
    if (glm::dot(EA,normal)>0) {
        direction=ABC;
    }
    else {
        normal=-normal;
        direction=ACB;
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