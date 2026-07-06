//
// Created by Mathis Pean on 09/06/2026.
//

#include "ObjParser.hpp"

#include <array>
#include "glm/vec2.hpp"
#include "model/Scene.h"
#include "model/Node.h"
#include "model/MeshBuilder.h"
#include <sstream>

#include "MtlParser.hpp"

glm::vec3 xzy(const glm::vec3 v)
{
    return glm::vec3(v.x, v.z, v.y);
}

ObjParser::ObjParser(const std::string &file,Scene* scene) : Parser(file, scene)
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

void ObjParser::parse(const std::string& file, Scene* scene)
{
    auto instance = ObjParser(file, scene);
    instance.parseImpl();
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
        if (mCurrent.type != IDENTIFIER) {
            if (mCurrent.type!=NEWLINE){
                throwError((std::stringstream() << "Token non attendu : " << mCurrent.type ).str());
                return;}
            next();
            continue;
        }
        if (mCurrent.identifier=="v") {
            parseV();
            next();
        }
        else if (mCurrent.identifier=="f"){
            parseF();
            next();
        }
        else if (mCurrent.identifier=="vn"){
            parseVN();
            next();
        }
        else if (mCurrent.identifier=="vt"){
            parseVT();
            next();
        }
        else if (mCurrent.identifier=="l"){
            parseL();
            next();
        }
        else if (mCurrent.identifier=="s"){
            parseS();
            next();
        }
        else if (mCurrent.identifier=="mtllib"){
            parseMtllib();
            next();
        }
        else if (mCurrent.identifier=="usemtl"){
            parseUsemtl();
            next();
        }
        else if (mCurrent.identifier=="g"){
            if (!gEncountered) {
                if ( mMeshBuildData->renderVertices.empty()) {
                    removeDefaultMesh();   // ← le 1er g supprime le mesh par défaut
                }
                else {mDefaultMeshNode=nullptr;}
                gEncountered = true;
            }
            parseG();
            next();
        }
        else if (mCurrent.identifier=="o"){
            parseO();
            next();
        }
        else {
            throwError((std::stringstream() << "Identifiant inconnu : " << mCurrent.identifier ).str());
            return;
        }
    }
    finishMesh();
}

void ObjParser::notEnoughComponentsError(int i) const {
    throwError((std::stringstream() << " Nombre de composantes incorrect : " << i ).str());
}

void ObjParser::parseV() {
    glm::vec3 v;
    int coord=0;
    next();
    while (mCurrent.type != NEWLINE && mCurrent.type != END ){//TODO les commentaires en fin de ligne enculent cela
        if (coord < 3) {
            if (mCurrent.type==FLOAT) {
                v[coord]= mCurrent.value.floatValue;
            }
            else if (mCurrent.type==INT) {
                v[coord]= mCurrent.value.intValue;
            }
            else {
                throwError("Expected a number");
            }
        }
        else {
            notEnoughComponentsError(4);
        }
        coord++;
        next();
    }
    if (coord!=3) {
        notEnoughComponentsError(coord);
        return;
    }
    mV.push_back(xzy(v));
}

void ObjParser::parseVN() {
    glm::vec3 vn;
    int coord=0;
    next();
    while (mCurrent.type != NEWLINE && mCurrent.type != END ){//TODO les commentaires en fin de ligne enculent cela
        if (coord < 3 ) {
            if (mCurrent.type==FLOAT) {
                vn[coord]= mCurrent.value.floatValue;
            }
            else if (mCurrent.type==INT) {
                vn[coord]= mCurrent.value.intValue;
            }
            else {
                throwError("Expected a number");
            }
        }
        else {
            notEnoughComponentsError(4);
        }
        coord++;
        next();
    }
    if (coord!=3) {
        notEnoughComponentsError(coord);
        return;
    }
    mVN.push_back( xzy(glm::normalize(vn)));
}

void ObjParser::parseVT() {
    glm::vec3 vt;
    int coord=0;
    next();
    while (mCurrent.type != NEWLINE && mCurrent.type != END ){//TODO les commentaires en fin de ligne enculent cela
        if (coord < 3 ) {
            if (mCurrent.type==FLOAT) {
                vt[coord]= mCurrent.value.floatValue;
            }
            else if (mCurrent.type==INT) {
                vt[coord]= mCurrent.value.intValue;
            }
            else {
                throwError("Expected a number");
            }
        }
        else {
            notEnoughComponentsError(4);
        }
        coord++;
        next();
    }

    mVT.push_back(glm::vec2(vt));
}


void ObjParser::parseO() {
    next();
    std::string name="";
    if (mCurrent.type==IDENTIFIER) {
        name=mCurrent.identifier;
        next();
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
    std::string name="";
    while (mCurrent.type==IDENTIFIER)
    {
        if (name!="") {
            name+=" ";
        }
        name+=mCurrent.identifier;
        next();
    }
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
        int v=-1;
        int vn=-1;
        int vt=-1;
        if (mCurrent.type!=INT) {
            break;
        }
        v=mCurrent.value.intValue-1;
        next();
        if (mCurrent.type==SLASH) {
            next();
            if (mCurrent.type==INT) {
                vt=mCurrent.value.intValue-1;
                next();
            }

            if (mCurrent.type==SLASH) {
                next();
                if (mCurrent.type==INT) {
                    vn=mCurrent.value.intValue-1;
                    next();
                }
            }

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
        default:
            throwError((std::stringstream() << " Nombre de sommets incorrect : " << nVertex ).str());
            break;
    }
}
void ObjParser::parseMtllib() {
    next();
    std::string filename="";
    while (mCurrent.type != NEWLINE && mCurrent.type != END) {
        filename += mCurrent.identifier;   // accumule tous les tokens
        next();
    }
    MtlParser::parse(mDir + filename, mScene);
}

void ObjParser::parseUsemtl() {
    next();
    if (mCurrent.type   == IDENTIFIER) {
        std::string name="";
        while (mCurrent.type != NEWLINE && mCurrent.type != END) {
            name += mCurrent.identifier;   // accumule tous les tokens
            next();
        }
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

