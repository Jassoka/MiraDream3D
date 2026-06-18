//
// Created by Mathis Pean on 09/06/2026.
//

#include "objParsing.hpp"

#include <array>
#include <iostream>
#include <cctype>

#include "glm/vec2.hpp"
#include "model/Scene.h"
#include <sstream>

//accepte 1 à 9, ., - ;
bool isNumber(const char c) {
    return '0'<=c && c<='9' ;
}
//accepte les lettres maj/min et le _
bool isLetter(const char c) {
    return ('a'<=c && c<='z') || ('A'<=c && c<='Z')  ;
}


ObjToken ObjLexer::next() {
    readSpace();
    if (mPos>=mSrc.size()) {
        return ObjToken{.type=END};
    }
    char c=mSrc[mPos];
    if (c=='\n') {
        skipLine();
        return(ObjToken{.type=NEWLINE});
    }
    if (c=='/') {
        mPos++; mCol++;
        return ObjToken{.type=SLASH};
    }
    if (c=='#') {
        skipLine();
        return next();
    }
    if (isLetter(c)|| c=='_'){return readIdentifier();}
    if (isNumber(c) || c=='-' || c=='.'||c=='+' ){return readNumber();}

    std::cout << "error char :" << c << std::endl;
    error("Unknown grammar");

}
ObjToken ObjLexer::readIdentifier() {
    char c=mSrc[mPos];

    ObjToken token;
    token.type=IDENTIFIER;

    uint32_t stringPos=0;
    while (mPos < mSrc.size() &&
       (isLetter(mSrc[mPos]) || isNumber(mSrc[mPos]) || mSrc[mPos]=='_'))
    {
        token.identifier += mSrc[mPos];
        ++mPos;
        ++mCol;
    }
    return token;
}
//voir grammaire discord
ObjToken ObjLexer::readNumber(){
    char c=mSrc[mPos];
    ObjToken token;

    bool isValid=false;
    bool isFloat=false;
    bool isNegative=false;
    bool hasSign=false;
    bool hasExp=false;
    bool isExpNeg=false;
    int exponent=0;
    int distanceFromComma=1;

    float number=0;
    while (mPos<mSrc.size() && (isNumber(c) || c=='E' || c=='e' || c=='-'||c=='+'||c=='.')) {
        if (c=='.') {
            isValid=false;
            if (isFloat) {
                break;
            }
            isFloat = true;
        }
        else if (c=='-') {
            isValid=false;
            if (hasSign) {
                break;
            }
            if (hasExp) {
                isExpNeg=true;
            }
            else {
                isNegative=true;
            }
            hasSign=true;

        }
        else if (c=='+') {
            isValid=false;
            if (hasSign) {
                break;
            }
            hasSign=true;
        }
        else if (c=='e' || c=='E') {
            isValid=false;
            if (hasExp) {
                break;
            }
            isFloat=true;
            hasSign=false;
            hasExp=true;
        }
        else {
            isValid=true;
            const int chiffre=c-'0';
            if (hasExp) {
                exponent*=10;
                exponent+=chiffre;
            }
            else if (isFloat) {
                number+=chiffre/std::pow(10,distanceFromComma);
                distanceFromComma++;
            }
            else {
                number*=10;
                number+=chiffre;
            }
        }


        mPos++;
        mCol++;
        c=mSrc[mPos];
    }
    if (!isValid)  error("Not a number ");
    if (hasExp) {
        if (isExpNeg) exponent=-exponent;
        number*=std::pow(10,exponent);
    }
    if (isNegative) number=-number;
    if (isFloat) {
        token.value.floatValue=number;
        token.type=FLOAT;
    }
    else {
        token.value.intValue=static_cast<int>(number);
        token.type=INT;
    }
    return token;
}


void ObjLexer::readSpace() {
    while (mPos < mSrc.size() &&
           std::isspace(static_cast<unsigned char>(mSrc[mPos])) &&
           mSrc[mPos] != '\n') {
        mPos++;
        mCol++;
           }
}

void ObjLexer::skipLine() {
    while (mPos < mSrc.size() && mSrc[mPos] != '\n')
        mPos++;
    mLin++;
    mCol=0;
    mPos++;
}



void ObjLexer::error(const std::string &msg) const {
    throw ObjLexerException(msg,mLin,mCol);
}

void ObjParser::parse() {
    next();


    //bool oEncountered=false;
    bool gEncountered=false;
    mCurrentNode=mScene->getRootNode();
    mCurrentMesh=mScene->newMesh();
    mDefaultMeshNode = dynamic_cast<Node*>(new MeshNode("",mScene->getMeshes().size()-1));
    dynamic_cast<HierarchyNode*>(mCurrentNode)->addChild(mDefaultMeshNode);





    while (mCurrent.type != END ) {
        if (mCurrent.type != IDENTIFIER) {
            if (mCurrent.type!=NEWLINE){
                error((std::stringstream() << "Token non attendu : " << mCurrent.type ).str());
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
                if ( mCurrentMesh->getVertices().empty()) {
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
            error((std::stringstream() << "Identifiant inconnu : " << mCurrent.identifier ).str());
            return;
        }
    }
    finishMesh();
}

void ObjParser::notEnoughComponentsError(int i) const {
    error((std::stringstream() << " Nombre de composantes incorrect : " << i ).str());
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
                error("Expected a number");
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
    mV.push_back(v);
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
                error("Expected a number");
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
    mVN.push_back(glm::normalize(vn));
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
                error("Expected a number");
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
        mCurrentMesh = mScene->newMesh();
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
    Face renderFace;
    Face geomFace;

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

        if (mCurrentMeshHasNormals && vn==-1){mCurrentMeshHasNormals=false;}
        if (mCurrentMeshHasUVCoords && vt==-1){mCurrentMeshHasUVCoords=false;}
        mCurrentMesh->mRenderVertices.push_back(RenderVertex(
                mV[v],
                glm::vec3(0.0),
                (vt==-1) ? glm::vec2(0.0) : mVT[vt]
            ));

        mCurrentMesh->mUserNormals.push_back((vn==-1) ? glm::vec3(0.0) : mVN[vn]);
        mCurrentMesh->mSmoothingGroups.push_back(mCurrentMeshSmoothGroupsMap[mCurrentSmoothGroup]);

        //creation du geomvertx s'il n'existe pas
        if (mCurrentMeshGeometricVerticesMap.find(v)==mCurrentMeshGeometricVerticesMap.end()) {
            mCurrentMesh->mGeometricVertices.push_back(GeometricVertex {});
            mCurrentMeshFacePerGeometricVertex.push_back(std::vector<uint32_t>());
            mCurrentMeshGeometricVerticesMap[v]=mCurrentMesh->mGeometricVertices.size()-1;
        }

        mCurrentMesh->getGeometricVertices()[mCurrentMeshGeometricVerticesMap[v]].vertices.push_back(mCurrentMesh->getVertices().size() - 1);
        renderFace[nVertex]=mCurrentMesh->mRenderVertices.size()-1;
        geomFace[nVertex] = mCurrentMeshGeometricVerticesMap[v];
        nVertex++;
    }
    const uint32_t faceID = mCurrentMesh->mRenderFaces.size(); // id que la face aura une fois ajoutée
    for (int i = 0; i < nVertex; i++) {
        mCurrentMeshFacePerGeometricVertex[geomFace[i]].push_back(faceID);
    }
    switch (nVertex) {
        case(3):
            mCurrentMesh->addTriangle(geomFace,renderFace);
            break;
        case(4):
            mCurrentMesh->addQuad(geomFace,renderFace);
            break;
        default:
            error((std::stringstream() << " Nombre de sommets incorrect : " << nVertex ).str());
            break;
    }
}
//TODO implemeter pour de vrai
void ObjParser::parseUsemtl() {
    next();
    while (mCurrent.type!=NEWLINE)
    {
        next();
    }
}
//TODO implemeter pour de vrai
void ObjParser::parseMtllib() {
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
            if (mCurrentMesh->mRenderVertices.empty()) {
                mCurrentMeshSmoothGroupsMap.clear();
                mCurrentMeshSmoothGroupsMap[0]=0;
                mCurrentMesh->nSmoothGroups=0;
            }
        }
        else error("smoothing group error");
    }
    else if (mCurrent.type == INT) {
        if (mCurrent.value.intValue>255  ||mCurrent.value.intValue<0) {
            error("too high smoothing group (>255), changed to 0.");//TODO warning
            mCurrent.value.intValue=0;

        }

        mCurrentSmoothGroup=static_cast<uint8_t>(mCurrent.value.intValue);
        if (mCurrentMeshSmoothGroupsMap.find(mCurrent.value.intValue) == mCurrentMeshSmoothGroupsMap.end()) {
            mCurrentMesh->nSmoothGroups++;
            mCurrentMeshSmoothGroupsMap[mCurrent.value.intValue] = mCurrentMesh->nSmoothGroups;
        }


    }
    else error("smoothing group error");
    next();
}


void ObjParser::removeDefaultMesh() {
    // retirer le MeshNode du parent
    auto* parent = dynamic_cast<HierarchyNode*>(mCurrentNode);
    parent->removeLastChild();   // à ajouter dans HierarchyNode
    delete dynamic_cast<MeshNode*>(mDefaultMeshNode);

    // retirer le mesh de la scène
    mScene->removeLastMesh();        // à ajouter dans Scene

    mCurrentMesh = nullptr;
    mDefaultMeshNode = nullptr;
}

void ObjParser::finishMesh() {
    mCurrentMesh->triangulate();
    mCurrentMesh->generateHalfEdges(&mCurrentMeshFacePerGeometricVertex);
    mCurrentMeshFacePerGeometricVertex.clear();
    mCurrentMeshGeometricVerticesMap.clear();
    mCurrentMeshSmoothGroupsMap.clear();


    mCurrentMeshHasNormals=true;
    mCurrentMeshHasUVCoords=true;
}
void ObjParser::createMesh(std::string name) {
    mCurrentMesh=mScene->newMesh();

    mCurrentMeshSmoothGroupsMap[0]=0;

    if (mCurrentSmoothGroup!=0) {
        mCurrentMeshSmoothGroupsMap[mCurrentSmoothGroup]=1;
        mCurrentMesh->nSmoothGroups++;
    }
}

void ObjParser::error(const std::string &msg) const {
    throw ObjParserException(msg,mLexer.getLine(),mLexer.getLine());
}