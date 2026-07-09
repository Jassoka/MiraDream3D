//
// Created by Mathis Pean on 09/06/2026.
//

#include "../../../include/model/importer/ObjParser.hpp"

#include <array>
#include "glm/vec2.hpp"
#include "model/Scene.h"
#include "model/Node.h"
#include "model/MeshBuilder.h"
#include <sstream>

#include "../../../include/model/importer/MtlParser.hpp"

glm::vec3 xzy(const glm::vec3 v)
{
    return glm::vec3(v.x, v.z, v.y);
}

void ObjParser::initFlags()
{
    mMeshBuildFlags->computedFacesAndVertices = true;
    mMeshBuildFlags->computedFacesPerVertex = true;
    mMeshBuildFlags->assignedMaterials = true;
}

ObjParser::~ObjParser()
{
    delete mMeshBuildFlags;
    delete mMeshBuildData;
}

void ObjParser::parseImpl() {
    next();

    //bool oEncountered=false;
    bool gEncountered=false;
    mCurrentNode=mScene->getRootNode();
    mCurrentMesh=mScene->newMesh();
    mDefaultMeshNode = dynamic_cast<Node*>(new MeshNode("",mScene->getMeshes().size()-1));
    dynamic_cast<HierarchyNode*>(mCurrentNode)->addChild(mDefaultMeshNode);
    mMeshBuildFlags->hasUserNormals=true;
    mCurrentMeshHasUVCoords=true;
    mCurrentMeshSmoothGroupsMap[0]=0;
    mMeshBuildData->nbSmoothingGroups=0;


    while (mCurrent.type != END ) {
        if (mCurrent.type != IDENTIFIER)
        {
            if (mCurrent.type != NEWLINE)
                throwError(ParserMessages::UnexpectedToken);
        }
        else
        {
            if (mCurrent.identifier=="v")
                parseV();
            else if (mCurrent.identifier=="f")
                parseF();
            else if (mCurrent.identifier=="vn")
                parseVN();
            else if (mCurrent.identifier=="vt")
                parseVT();
            else if (mCurrent.identifier=="l")
                parseL();
            else if (mCurrent.identifier=="s")
                parseS();
            else if (mCurrent.identifier=="mtllib")
                parseMtllib();
            else if (mCurrent.identifier=="usemtl")
                parseUsemtl();
            else if (mCurrent.identifier=="g"){
                if (!gEncountered) {
                    if ( mMeshBuildData->renderVertices.empty()) {
                        removeDefaultMesh();   // ← le 1er g supprime le mesh par défaut
                    }
                    else {mDefaultMeshNode=nullptr;}
                    gEncountered = true;
                }
                parseG();
            }
            else if (mCurrent.identifier=="o"){
                parseO();
            }
            else {
                throwError(ParserMessages::UnexpectedIdentifier, mCurrent.identifier);
            }
        }
        expectEOL();
    }
    finishMesh();
}

void ObjParser::parseV() {
    next();
    mV.push_back(xzy(parseVec3()));
}

void ObjParser::parseVN() {
    next();
    mVN.push_back(xzy(glm::normalize(parseVec3())));
}

void ObjParser::parseVT() {
    next();
    mVT.push_back(parseVec2());
}


void ObjParser::parseO() {
    next();
    std::string name="";
    if (mCurrent.type==IDENTIFIER) {
        name = parseName();
    }
    auto* newNode = new HierarchyNode(name);

    // si le mesh par défaut existe encore, le déplacer dans ce nouvel objet
    if (mDefaultMeshNode != nullptr) {
        dynamic_cast<HierarchyNode*>(mCurrentNode)->popLastChild();
        newNode->addChild(mDefaultMeshNode);
        mDefaultMeshNode = nullptr;

    }else {
        // Crée un mesh propre pour ce nouvel objet
        finishMesh();
        createMesh(name);
        auto* meshNode = new MeshNode(name, mScene->getMeshes().size() - 1);
        newNode->addChild(meshNode);
    }

    mCurrentNode = newNode;
    dynamic_cast<HierarchyNode*>(mScene->getRootNode())->addChild(mCurrentNode);
}

void ObjParser::parseG() {
    next();
    const std::string name= parseName();
    /*
    while (mCurrent.type==IDENTIFIER)
    {

        if (name!="") {
            name+=" ";
        } //TODO jsp ce que ça fait
        name+=mCurrent.identifier;
        next();
    }*/
    if (mCurrentMesh) finishMesh();
    createMesh(name);
    dynamic_cast<HierarchyNode *>(mCurrentNode)->addChild(new MeshNode(name,mScene->getMeshes().size()-1));
}


void ObjParser::parseF() {
    uint nVertex=0;
    next();
    Face renderFace{};
    Face geomFace{};

    while (mCurrent.type != NEWLINE && mCurrent.type != END ) {
        int vn=-1;
        int vt=-1;
        const int v = parseInt()-1;
        expectToken(SLASH);
        if (mCurrent.type!=SLASH) {
            vt = parseInt()-1;
        }
        expectToken(SLASH);
        if (mCurrent.type!=SLASH) {
            vn = parseInt()-1;
        }

        //quand on a fini de parser le point

        if (mMeshBuildFlags->hasUserNormals && vn==-1){mMeshBuildFlags->hasUserNormals=false;}
        if (mCurrentMeshHasUVCoords && vt==-1){mCurrentMeshHasUVCoords=false;}
        mMeshBuildData->renderVertices.push_back(RenderVertex(
                mV[v],
                glm::vec3(0.0),
                (vt==-1) ? glm::vec2(0.0) : mVT[vt]
            ));

        mMeshBuildData->userNormals.push_back((vn==-1) ? glm::vec3(0.0) : mVN[vn]);
        mMeshBuildData->smoothingGroups.push_back(mCurrentMeshSmoothGroupsMap[mCurrentSmoothGroup]);

        //creation du geomvertx s'il n'existe pas
        if (mCurrentMeshGeometricVerticesMap.find(v)==mCurrentMeshGeometricVerticesMap.end()) {
            mMeshBuildData->geometricVertices.push_back(GeometricVertex {});
            mMeshBuildData->facesPerVertex.push_back(std::vector<uint32_t>());
            mCurrentMeshGeometricVerticesMap[v]=mMeshBuildData->geometricVertices.size()-1;
        }

        mMeshBuildData->geometricVertices[mCurrentMeshGeometricVerticesMap[v]].vertices.push_back(mMeshBuildData->renderVertices.size() - 1);
        if (nVertex > MAX_FACESIZE-1)
        {
            throwError((std::stringstream() << " N-gon face with more than " << MAX_FACESIZE << " vertices." ).str());
        }
        renderFace[nVertex]=mMeshBuildData->renderVertices.size()-1;
        geomFace[nVertex] = mCurrentMeshGeometricVerticesMap[v];
        nVertex++;
    }
    const uint32_t faceID = mMeshBuildData->renderFaces.size(); // id que la face aura une fois ajoutée
    for (int i = 0; i < nVertex; i++) {
        mMeshBuildData->facesPerVertex[geomFace[i]].push_back(faceID);
    }
    switch (nVertex) {
        case(3):
        case(4):
            mMeshBuildData->addFace(geomFace, renderFace, nVertex);
            break;
    }
}
void ObjParser::parseMtllib() {
    next();
    const std::string filename= parseName();
    MtlParser::parse(mDir + filename, mScene, mWarnings);
}

void ObjParser::parseUsemtl() {
    next();
    if (mCurrent.type == IDENTIFIER) {
        const std::string name = parseName();
        mCurrentSubMesh = mMeshBuildData->newSubMesh(mScene->getMaterialID(name));
    }
}
//TODO implemeter pour de vrai
void ObjParser::parseL() {
    next();
    while (mCurrent.type!=NEWLINE)
    {
        next();
    }
}
void ObjParser::parseS() {
    next();

    if (mCurrent.type == IDENTIFIER) {
        if (mCurrent.identifier=="off") {
            mCurrentSmoothGroup=0;
            if (mMeshBuildData->renderVertices.empty()) {
                mCurrentMeshSmoothGroupsMap.clear();
                mCurrentMeshSmoothGroupsMap[0]=0;
                mMeshBuildData->nbSmoothingGroups=0;
            }
        }
        else throwError("smoothing group error");
    }
    else if (mCurrent.type == INT) {
        if (mCurrent.value.intValue>255  ||mCurrent.value.intValue<0) {
            throwError("too high smoothing group (>255), changed to 0.");//TODO warning
            mCurrent.value.intValue=0;

        }

        mCurrentSmoothGroup=static_cast<uint8_t>(mCurrent.value.intValue);
        if (mCurrentMeshSmoothGroupsMap.find(mCurrent.value.intValue) == mCurrentMeshSmoothGroupsMap.end()) {
            mMeshBuildData->nbSmoothingGroups++;
            mCurrentMeshSmoothGroupsMap[mCurrent.value.intValue] = mMeshBuildData->nbSmoothingGroups;
        }


    }
    else throwError("smoothing group error");
    next();
}


void ObjParser::removeDefaultMesh() {
    // retirer le MeshNode du parent
    auto* parent = dynamic_cast<HierarchyNode*>(mCurrentNode);
    //parent->removeLastChild();   // à ajouter dans HierarchyNode
    delete dynamic_cast<MeshNode*>(mDefaultMeshNode);

    // retirer le mesh de la scène
    mScene->removeLastMesh();        // à ajouter dans Scene

    mCurrentMesh = nullptr; //TODO euh jsp je touche pas
    mDefaultMeshNode = nullptr;
}

void ObjParser::finishMesh() {
    MeshBuilder::build(mCurrentMesh, *mMeshBuildData, *mMeshBuildFlags);
    delete mMeshBuildData;
    delete mMeshBuildFlags;
    mMeshBuildData = new MeshBuildData();
    mMeshBuildFlags = new MeshBuildFlags();
    mCurrentMeshGeometricVerticesMap.clear();
    mCurrentMeshSmoothGroupsMap.clear();
}
void ObjParser::createMesh(std::string name) {
    mCurrentMesh=mScene->newMesh();
    mMeshBuildFlags->hasUserNormals=true;
    mCurrentMeshHasUVCoords=true;
    mMeshBuildFlags->computedFacesAndVertices = true;
    mMeshBuildFlags->computedFacesPerVertex = true;
    mMeshBuildFlags->assignedMaterials = true;
    mCurrentMeshSmoothGroupsMap[0]=0;
    mMeshBuildData->nbSmoothingGroups=0;
    if (mCurrentSmoothGroup!=0) {
        mCurrentMeshSmoothGroupsMap[mCurrentSmoothGroup]=1;
        mMeshBuildData->nbSmoothingGroups++;
    }
}

