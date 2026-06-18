//
// Created by jassoka on 6/16/26.
//

#include "model/HalfEdgeBuilder.h"

#include <unordered_set>

#include "glm/detail/func_geometric.inl"
#include "model/Mesh.h"

halfEdgeDirection defaultHalfEdgeDirection = ABC;

HalfEdgeBuilder::HalfEdgeBuilder(Mesh& mesh, const std::vector<std::vector<uint32_t>>* facesPerVertex):
    mMesh(mesh), mFacesPerVertex(facesPerVertex) {}

void HalfEdgeBuilder::generateEdges()
{
    // On génère les edges normales également
    uint32_t edgeID = mEdges.size();
    auto &faces = mMesh.getGeometricFaces();
    for (int32_t fID = 0; fID < faces.size(); fID++)
    {
        Face f = faces[fID];
        const uint32_t faceSize = mMesh.getNbVertex(fID);
        for (uint32_t i = 0; i < faceSize; i++)
        {
            const uint32_t next_j = (i + 1)%faceSize;
            const uint32_t origin = f[i];
            const uint32_t end = f[next_j];
            uint64_t key = getEdgeMapKey(origin, end);
            if (auto it = mEdgeMap.find(key); it == mEdgeMap.end()) // not in map
            {
                mEdges.push_back(Edge{origin, end});
                mAdjacentFacesToEdge.push_back(std::array<int32_t, 2>());
                mAdjacentFacesToEdge[edgeID] = {fID, -1};
                mEdgeMap.insert(std::make_pair(key, edgeID++));
            }
            else // in map
            {
                mAdjacentFacesToEdge[it->second][1] = fID;
            }
        }
    }
}

void HalfEdgeBuilder::generateFacesPerVertex()
{
    auto &faces = mMesh.getGeometricFaces();
    mOwnedFacesPerVertex.resize(mMesh.getGeometricVertices().size());
    for (uint32_t fID = 0; fID < faces.size(); fID++)
    {
        Face f = faces[fID];
        for (uint32_t i = 0; i < mMesh.getNbVertex(fID); i++) {
            mOwnedFacesPerVertex[f[i]].push_back(fID);
        }
    }
    mFacesPerVertex = &mOwnedFacesPerVertex;
}

int32_t HalfEdgeBuilder::findVx()
{
    /* Trouver la liste des extremums en x */
    int firstIndex = 0;
    while (mVisitedVertex[firstIndex]) firstIndex++;
    float xMax = mMesh.getGeometricVertexPosition(firstIndex).x;
    for (uint32_t idxV = firstIndex; idxV < mMesh.getGeometricVertices().size(); idxV ++)
    {
        const float currX = mMesh.getGeometricVertexPosition(idxV).x;
        if (currX > xMax && !mVisitedVertex[idxV]) xMax = currX;
    }
    std::unordered_set<uint32_t> extremums;
    for (uint32_t idxV = firstIndex; idxV < mMesh.getGeometricVertices().size(); idxV ++)
    {
        const float currX =mMesh.getGeometricVertexPosition(idxV).x;
        if (xMax - currX < FLT_EPSILON && !mVisitedVertex[idxV]) extremums.insert(idxV);
    }
    // On cherche un point Vx tq il existe un voisin non extremum
    for (auto [origin, end] : mEdges)
    {
        if (extremums.find(origin) == extremums.end() && extremums.find(end) != extremums.end())
            return end;
        if (extremums.find(origin) != extremums.end() && extremums.find(end) == extremums.end())
            return origin;
    }
    return -1;
}

uint32_t HalfEdgeBuilder::generateFirstFaceHalfEdges(std::vector<uint32_t> &halfEdgesToIterate)
{
    // On suppose le mesh un manifold
    const int32_t Vx = findVx();
    uint32_t faceID;
    // Si il n'existe pas: alors c'est plat sur x, on prend n'importe quelle premiere normale
    if (Vx == -1)
    {
        faceID = 0;
        mNormalPerFace[faceID] = mMesh.getNormal(mMesh.mGeometricFaces[faceID], defaultHalfEdgeDirection);
    }
    // Sinon: soit Vx ce point, on nomme E la moyenne des points adjacents
    // On prend n'importe quelle face (0), et on a EE' (E' proj ortho de E sur la face) sa normale
    // Si E==E' alors n'importe quelle normale

    // Prendre la face en question: A, B, C ...
    // Alors si AB^AC de même sens à n, c'est ABC, sinon CBA
    else
    {
        faceID = (*mFacesPerVertex)[Vx][0];
        const halfEdgeDirection firstOrientation = mMesh.findFaceOrientation(Vx, (*mFacesPerVertex)[Vx], &mNormalPerFace[faceID]);
        if (firstOrientation != defaultHalfEdgeDirection) mMesh.swapFaceOrientation(faceID); // On oriente correctement les faces
    }
    // 1ere face
    const Face &currFace = mMesh.mGeometricFaces[faceID];
    const uint32_t faceSize = mMesh.getNbVertex(faceID);

    const uint32_t halfEdgeMax = mHalfEdges.size();
    mMesh.mComponents.push_back(halfEdgeMax); // First index of half edge list

    for (uint32_t i = 0; i < faceSize; i++)
    {
        halfEdgesToIterate.push_back(halfEdgeMax + i);
        const uint32_t next_i = (i+1)%faceSize;
        const uint32_t prev_i = (i + faceSize -1)%faceSize;
        const uint32_t origin = currFace[i];
        const uint32_t end = currFace[next_i];

        // On oriente les faces pour être dans le sens des half edges

        mHalfEdges.push_back(HalfEdge{halfEdgeMax + next_i, halfEdgeMax + prev_i, faceID, -1, origin, end});
        mVisitedVertex[origin] = 1;
    }
    return faceID;
}

void HalfEdgeBuilder::generateHalfEdges(uint32_t &facesToVisit)
{
    mEdgeIdToHalfEdges.resize(mEdges.size(), {-1, -1});
    std::vector<uint32_t> halfEdgesToIterate;
    // Propagation des half edges, jusqu'a que chaque face soit touchée
    uint32_t faceID = generateFirstFaceHalfEdges(halfEdgesToIterate);
    uint32_t halfEdgeIterationIndex = 0; // attention tres different de halfEdgeIdx ,c'est l'indice de la liste ci-haut, et non pas l'indice courant de mHalfEdges


    mVisitedFace[faceID] = 1;
    facesToVisit--;
    while (halfEdgeIterationIndex < halfEdgesToIterate.size()) // tant qu'on a des aretes à parcourir
    {
        const uint32_t currHalfEdgeIdx = halfEdgesToIterate[halfEdgeIterationIndex];
        HalfEdge currHalfEdge = mHalfEdges[currHalfEdgeIdx];

        const uint32_t edgeID = getEdgeMapValue(currHalfEdge.origin, currHalfEdge.end);

        // Trouver la face voisine (il y en a au plus 1 par la propriété du mesh 2-manifold //TODO verifier que c'est bien 2-manifold à un moment
        const int32_t neighbouringFace = [this, edgeID](const uint32_t face)
        {
            int32_t neighbour;
            if (mAdjacentFacesToEdge[edgeID][0] == face)
            {
                neighbour = mAdjacentFacesToEdge[edgeID][1];
            }
            else
            {
                neighbour = mAdjacentFacesToEdge[edgeID][0];
            }
            if (neighbour == -1) return -1;
            if (mVisitedFace[neighbour]) return -2;
            return neighbour;
        }(currHalfEdge.face);

        if (neighbouringFace == -1) // is a boundary edge
        {
            halfEdgeIterationIndex++;
            continue;
        }

        halfEdgeDirection faceOrientation;
        // la nouvelle half edge commence dans l'autre sens
        const uint32_t origin = currHalfEdge.end;
        const uint32_t end = currHalfEdge.origin;

        // On le rajoute dans la map pour trouver son twin si jamais il vient/ est venu
        const auto currHalfEdgePair = &mEdgeIdToHalfEdges[getEdgeMapValue(origin, end)];
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
        faceID = neighbouringFace;
        auto &face = mMesh.mGeometricFaces[faceID];

        mVisitedFace[faceID] = 1;
        facesToVisit--;


        const uint32_t faceSize = mMesh.getNbVertex(faceID);

        uint32_t startIndice = 0;

        for (uint32_t j = 0; j < faceSize; j++)
        {
            if (face[j] == origin)
            {
                if (face[(j+1) % faceSize] == end) faceOrientation = ABC;
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

            const uint32_t origin = face[(startIndice + i) % faceSize];
            const uint32_t end = face[(startIndice + (faceOrientation==ABC?next_i:prev_i)) % faceSize];

            mHalfEdges.push_back(HalfEdge{halfEdgeLast+next_i,
                halfEdgeLast+prev_i,
                faceID,
                -1,
                origin,
                 end
            });
            mVisitedVertex[origin] = 1;
        }

        if (faceOrientation != defaultHalfEdgeDirection) mMesh.swapFaceOrientation(faceID);
        mNormalPerFace[faceID] = mMesh.getNormal(face, defaultHalfEdgeDirection);
        halfEdgeIterationIndex++;
    }
}

void HalfEdgeBuilder::buildImpl()
{
    if (!mFacesPerVertex)
        generateFacesPerVertex();
    generateEdges();
    const auto &faces = mMesh.getGeometricFaces();
    uint32_t facesToVisit = faces.size();
    mNormalPerFace.resize(faces.size());

    mVisitedFace.resize(faces.size());
    mVisitedVertex.resize(mMesh.mGeometricVertices.size());

    //std::vector<glm::vec3> normalPerFace(faces.size());
    while (facesToVisit != 0)
    {
        generateHalfEdges(facesToVisit);
    }
    generateNormals();
}

void HalfEdgeBuilder::generateNormals() const
{
    for (uint32_t gIdx = 0; gIdx < mMesh.mGeometricVertices.size(); gIdx++)
    {
        auto [vertices, halfEdge] = mMesh.mGeometricVertices[gIdx];
        std::vector<std::vector<uint32_t>> smoothingGroups(mMesh.nSmoothGroups);
        //TODO trouver une solution pour rajouter un half edge par geometric vertex
        for (uint32_t vIdx = 0; vIdx < vertices.size(); vIdx++)
        {
            const uint32_t faceID = (*mFacesPerVertex)[gIdx][vIdx];
            const auto hardNormal = mNormalPerFace[faceID];
            const auto renderVertexID = vertices[vIdx];
            mMesh.mHardNormals[renderVertexID] = hardNormal;
            if (!mMesh.hasNormals)
            {
                if (!mMesh.isSmooth())
                    mMesh.mRenderVertices[renderVertexID].setNormal(hardNormal);
                else
                {
                    smoothingGroups[mMesh.mSmoothingGroups[renderVertexID]].push_back(renderVertexID);
                }
            }
            else
            {

                const auto userNormal = mMesh.mUserNormals[renderVertexID];
                mMesh.mRenderVertices[renderVertexID].setNormal(userNormal);
            }
        }
        if (!mMesh.isSmooth()) //TODO ce serait pratique de mettre ça ailleurs pour réutiliser
        {
            std::vector<glm::vec3> smoothGroupAverageNormals(mMesh.nSmoothGroups);
            for (int i = 0; i < mMesh.nSmoothGroups; i++)
            {
                glm::vec3 normalSum(0.0);
                for (const auto renderVertexID : smoothingGroups[i])
                {
                    normalSum += mMesh.mRenderVertices[renderVertexID].getNormal();
                }
                const auto smoothNormal = glm::normalize(normalSum);
                for (const auto renderVertexID : smoothingGroups[i])
                {
                    mMesh.mRenderVertices[renderVertexID].setNormal(smoothNormal);
                }

            }
        }
    }
}

void HalfEdgeBuilder::build(Mesh& mesh, const std::vector<std::vector<uint32_t>>* facesPerVertex)
{
    HalfEdgeBuilder builder(mesh, facesPerVertex);
    builder.buildImpl();
}
