//
// Created by Mathis Pean on 09/06/2026.
//

#include "objParsing.hpp"

#include <array>
#include <iostream>

#include "glm/vec2.hpp"
#include "model/Scene.h"


//accepte 1 à 9, ., - ;
bool isNumber(char &c) {
    return '0'<=c && c<='9' ;
}
//accepte les lettres maj/min et le _
bool isLetter(char &c) {
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

    return (errorToken());
}
ObjToken ObjLexer::readIdentifier() {
    char c=mSrc[mPos];

    ObjToken token;
    token.type=IDENTIFIER;

    uint32_t stringPos=0;
    while (mPos<mSrc.size() && (isLetter(c) || isNumber(c) || c=='_')) {
        token.identifier+=c;
        mPos++;
        mCol++;
        c=mSrc[mPos];
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
    if (!isValid) return errorToken();
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

void ObjLexer::readSpace(){
    char c = mSrc[mPos];
    while (mPos<mSrc.size() && c==' ') {
        mPos++;
        mCol++;
        c=mSrc[mPos];
    }
}

void ObjLexer::skipLine() {
    while (mPos < mSrc.size() && mSrc[mPos] != '\n')
        mPos++;
    mLin++;
    mCol=0;
    mPos++;
}



ObjToken ObjLexer::errorToken() {
    std::cout <<"mot clef inconnu dans le Lexer"<<std::endl;
    return ObjToken{.type=UNKNOWN};
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
                //TODO explosion
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
        else if (mCurrent.identifier=="g"){
            if (!gEncountered) {
                if (!gEncountered && mCurrentMesh->getVertices().empty()) {
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
            //TODO explosion
            return;
        }
    }
    finishMesh();
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
                //TODO explosion
            }
        }
        else {
            //TODO explosion
        }
        coord++;
        next();
    }
    if (coord!=3) {
        //TODO explosion
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
                //TODO explosion
            }
        }
        else {
            //TODO explosion
        }
        coord++;
        next();
    }
    if (coord!=3) {
        //TODO explosion
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
                //TODO explosion
            }
        }
        else {
            //TODO explosion
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
        mCurrentMeshGeometricVerticesMap.clear();
        mCurrentMeshFacePerGeometricVertex.clear();
        mCurrentMeshHasUVCoords = true;
        mCurrentMeshHasNormals = true;
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
    mCurrentMesh=mScene->newMesh();
    dynamic_cast<HierarchyNode *>(mCurrentNode)->addChild(new MeshNode(name,mScene->getMeshes().size()-1));
    mCurrentMeshFacePerGeometricVertex.clear();
    mCurrentMeshGeometricVerticesMap.clear();
    mCurrentMeshHasUVCoords=true;
    mCurrentMeshHasNormals=true;
}


void ObjParser::parseF() {
    uint nVertex=0;
    next();
    Face face;

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
        mCurrentMesh->addVertex(Vertex(
            mV[v],
            (vn==-1) ? glm::vec3(0.0) : mVN[vn],
                (vt==-1) ? glm::vec2(0.0) : mVT[vt]
            ));
        //creation du geomvertx s'il n'existe pas
        if (mCurrentMeshGeometricVerticesMap.find(v)==mCurrentMeshGeometricVerticesMap.end()) {
            mCurrentMesh->addGeometricVertex(GeometricVertex {});
            mCurrentMeshFacePerGeometricVertex.push_back(std::vector<uint32_t>());
            mCurrentMeshGeometricVerticesMap[v]=mCurrentMesh->getGeometricVertices().size()-1;
        }

        mCurrentMesh->getGeometricVertices()[mCurrentMeshGeometricVerticesMap[v]].vertices.push_back(mCurrentMesh->getVertices().size() - 1);
        face[nVertex]=mCurrentMesh->getVertices().size()-1;
        nVertex++;

        for (int i = 0;i<nVertex;i++) {
            mCurrentMeshFacePerGeometricVertex[mCurrentMeshGeometricVerticesMap[face[i]]].push_back(face[i]);
        }
    }

    switch (nVertex) {
        case(3):
            mCurrentMesh->addTriangle(face);
            break;
        case(4):
            mCurrentMesh->addQuad(face);
            break;
        default:
            return;//TODO explosion
            break;
    }
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
    mCurrentMesh->generateHalfEdges(mCurrentMeshFacePerGeometricVertex);
}