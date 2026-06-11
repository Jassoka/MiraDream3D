#include "model/Mesh.h"

#include <iostream>
#include <unordered_map>
#include <assimp/include/assimp/mesh.h>
#include <assimp/vector3.h>
#include <sys/stat.h>

#include "glm/geometric.hpp"

Mesh::Mesh(uint32_t MaterialID):mMaterialID(MaterialID) {

}
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

    std::vector<uint8_t> visitedFace(mFaces.size(), 0);
    std::vector<uint8_t> visitedVertex(mVertices.size(), 0);
    uint32_t facesToVisit = mFaces.size();


    mHalfEdges.clear();

    //TODO FUSIONNER LES VERTEX DE MEME POINT (vertex géometriques) (ça implique des faces géométriques distinctes aussi)
    //TODO chaque vertex geometrique stocke une half edge qui lui appartient pour O(1) lookup
    std::vector<std::array<int32_t, 2>> adjacentFacesToEdge;
    std::unordered_map<uint64_t, uint32_t> edgeMap;

    auto getEdgeMapKey = [](const uint32_t firstVertex, const uint32_t secondVertex) -> uint64_t {
        const uint32_t origin = std::min(firstVertex, secondVertex);
        const uint32_t end = std::max(firstVertex, secondVertex);
        return static_cast<uint64_t>(origin) << 32 | end;
    };

    auto getEdgeMapValue = [&edgeMap, &getEdgeMapKey](const uint32_t firstVertex, const uint32_t secondVertex) -> uint32_t {
        const auto it = edgeMap.find(getEdgeMapKey(firstVertex, secondVertex));
        if (it == edgeMap.end()) return -1;
        return it->second;
    };

    [this, &adjacentFacesToEdge, &edgeMap, &getEdgeMapKey]
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
                const uint32_t origin = f[i];
                const uint32_t end = f[next_j];
                uint64_t key = getEdgeMapKey(origin, end);
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


    auto facesPerVertex = std::vector<std::vector<uint32_t>>(mVertices.size());
    for (uint32_t fID = 0; fID < mFaces.size(); fID++)
    {
        Face &f = mFaces[fID];
        for (uint32_t i = 0; i < getNbVertex(fID); i++) {
            facesPerVertex[f[i]].push_back(fID);
        }
    }

    while (facesToVisit != 0)
    {
        // On suppose le mesh un manifold
        /* Trouver la liste des extremums en x */
        int firstIndex = 0;
        while (visitedVertex[firstIndex]) firstIndex++;
        float xMax = mVertices[firstIndex].x;
        for (uint32_t idxV = firstIndex; idxV < mVertices.size(); idxV ++)
        {
            if (mVertices[idxV].x > xMax && !visitedVertex[idxV]) xMax = mVertices[idxV].x;
        }
        std::unordered_set<uint32_t> extremums;
        for (uint32_t idxV = firstIndex; idxV < mVertices.size(); idxV ++)
        {
            if (xMax - mVertices[idxV].x < FLT_EPSILON && !visitedVertex[idxV]) extremums.insert(idxV);
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

        std::vector<glm::vec3> normalPerFace(mFaces.size()); //TODO envoyer les normales dans ce vecteur
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

        const uint32_t halfEdgeMax = mHalfEdges.size();
        mComponents.push_back(halfEdgeMax); // First index of half edge list
        auto edgeIdToHalfEdges = std::vector<std::array<int32_t, 2>>(mEdges.size(), {-1, -1}); // Map reliant les edges à ses 2 half edges, -1 si pas encore trouvée

        for (uint32_t i = 0; i < faceSize; i++)
        {
            halfEdgesToIterate.push_back(halfEdgeMax + i);
            const uint32_t next_i = (i+1)%faceSize;
            const uint32_t prev_i = (i + faceSize -1)%faceSize;
            const uint32_t origin = currFace[i];
            const uint32_t end = currFace[(firstOrientation == ABC) ? next_i : prev_i];

            mHalfEdges.push_back(HalfEdge{halfEdgeMax + next_i, halfEdgeMax + prev_i, faceID, -1, origin, end});
            visitedVertex[origin] = 1;
        }
        // Propagation des half edges, jusqu'a que chaque face soit touchée

        visitedFace[faceID] = 1;
        facesToVisit--;

        while (halfEdgeIterationIndex < halfEdgesToIterate.size()) // tant qu'on a des aretes à parcourir
        {
            const uint32_t currHalfEdgeIdx = halfEdgesToIterate[halfEdgeIterationIndex];
            HalfEdge currHalfEdge = mHalfEdges[currHalfEdgeIdx];

            const uint32_t edgeID = [&getEdgeMapValue](const uint32_t a, const uint32_t b)
            {
                return getEdgeMapValue(a, b);
            }(currHalfEdge.origin, currHalfEdge.end);

            // Trouver la face voisine (il y en a au plus 1 par la propriété du mesh 2-manifold //TODO verifier que c'est bien 2-manifold à un moment
            const int32_t neighbouringFace = [&adjacentFacesToEdge, edgeID, &visitedFace](const uint32_t face)
            {
                int32_t neighbour;
                if (adjacentFacesToEdge[edgeID][0] == face)
                {
                    neighbour = adjacentFacesToEdge[edgeID][1];
                }
                else
                {
                    neighbour = adjacentFacesToEdge[edgeID][0];
                }
                if (neighbour == -1) return -1;
                if (visitedFace[neighbour]) return -2;
                return neighbour;


            }(currHalfEdge.face);

            if (neighbouringFace == -1) // is a boundary edge
            {
                halfEdgeIterationIndex++;
                continue;
            }

            halfEdgeDirection faceOrientation = ABC;
            // la nouvelle half edge commence dans l'autre sens
            uint32_t origin = currHalfEdge.end;
            uint32_t end = currHalfEdge.origin;

            // On le rajoute dans la map pour trouver son twin si jamais il vient/ est venu
            auto currHalfEdgePair = &edgeIdToHalfEdges[getEdgeMapValue(origin, end)];
            if ((*currHalfEdgePair)[0] == -1) // Cas: cette arête n'a jamais été vue
            {
                (*currHalfEdgePair)[0] = currHalfEdgeIdx;
            }
            else // On l'a déjà vue: twins se forment
            {
                mHalfEdges[(*currHalfEdgePair)[0]].twin = currHalfEdgeIdx;
                mHalfEdges[currHalfEdgeIdx].twin = (*currHalfEdgePair)[0];
            }

            if (neighbouringFace == -2) // si déjà visité, on tente quand meme de former des twins
            {
                halfEdgeIterationIndex++;
                continue;
            }


            // On propage sur la nouvelle face
            currFace = mFaces[neighbouringFace];

            visitedFace[neighbouringFace] = 1;
            facesToVisit--;


            uint32_t faceSize = getNbVertex(neighbouringFace);

            uint32_t startIndice = 0;

            for (uint32_t j = 0; j < faceSize; j++)
            {
                if (currFace[j] == origin)
                {
                    if (currFace[(j+1) % faceSize] == end) faceOrientation = ABC;
                    else faceOrientation = ACB;
                    startIndice = j;
                }
            }


            const uint32_t halfEdgeLast = mHalfEdges.size();

            for (uint32_t i = 0; i < faceSize; i++)
            {
                const uint32_t next_i = (i+1)%faceSize;
                const uint32_t prev_i = ((i + faceSize - 1))%faceSize;
                halfEdgesToIterate.push_back(mHalfEdges.size());

                const uint32_t origin = currFace[(startIndice + i) % faceSize];
                const uint32_t end = currFace[(startIndice + (faceOrientation==ABC?next_i:prev_i)) % faceSize];

                mHalfEdges.push_back(HalfEdge{halfEdgeLast+next_i,
                    halfEdgeLast+prev_i,
                     static_cast<uint32_t>(neighbouringFace),
                    -1,
                    origin,
                    end
                });
                visitedVertex[origin] = 1;
            }
            halfEdgeIterationIndex++;
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
    std::cout << *this << std::endl;
}

void Mesh::findNormalAndOrientation(uint32_t AId,const std::vector<uint32_t> &adjacentFaces, glm::vec3 &normal, halfEdgeDirection &direction)
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

    const glm::vec3 A=glm::vec3(AVertex.x,AVertex.y,AVertex.z);
    const glm::vec3 B=glm::vec3(BVertex.x,BVertex.y,BVertex.z);
    const glm::vec3 C=glm::vec3(CVertex.x,CVertex.y,CVertex.z);
        //AB^AC normalisé
    return(glm::normalize(glm::cross(B-A,C-A)));
}