//
// Created by Mathis Pean on 19/06/2026.
//

#include "MtlParser.hpp"
#include <filesystem>

#include "model/Scene.h"


MtlParser::MtlParser(const std::string &file,Scene* scene):
    mLexer(ObjLexer( readFileToString(file))),
    mScene(scene),
    mDir(std::filesystem::path(file).parent_path().string() + "/")
{}



void MtlParser::parse(const std::string& file, Scene* scene){
    auto instance = MtlParser(file, scene);
    instance.parseImpl();

}



void MtlParser::parseImpl() {
    next();
    while (mCurrent.type != END ) {
        if (mCurrent.type != IDENTIFIER) {
            if (mCurrent.type!=NEWLINE){
                //TODO error((std::stringstream() << "Token non attendu : " << mCurrent.type ).str());
                return;}
            next();
            continue;
        }
        if (mCurrent.identifier=="newmtl") {
            parseNewmtl();
            next();
        }
        else if (mCurrentMaterial==nullptr){return;}//TODO explosion
        else if (mCurrent.identifier=="Ks") {
            parseKs();
        }
        else if (mCurrent.identifier=="Ka") {
            parseKa();
        }
        else if (mCurrent.identifier=="Kd") {
            parseKd();
        }
        else if (mCurrent.identifier=="d") {
            parseD();
        }
        else if (mCurrent.identifier=="Tr") {
            parseTr();
        }
        else if (mCurrent.identifier=="Ns") {
            parseNs();
        }
        else if (mCurrent.identifier=="Map_Kd") {
            parseMap_Kd();
        }
        else {
            while (mCurrent.type !=NEWLINE) {
                next();
            }
            next();
        }

    }
}



void MtlParser::parseNewmtl() {

    next();
    if (mCurrent.type != IDENTIFIER) {
        //TODO erreur
    }
    mCurrentMaterial=mScene->giveNewMaterial(mCurrent.identifier);
    next();
}



void MtlParser::parseKs() {
    mCurrentMaterial->Ks=parseVec3();
}

void MtlParser::parseKd() {
    mCurrentMaterial->Kd=parseVec3();
}
void MtlParser::parseKa() {
    mCurrentMaterial->Ka=parseVec3();
}
void MtlParser::parseD() {
    next();
    if (mCurrent.type == FLOAT ) {
        mCurrentMaterial->alpha=mCurrent.value.floatValue;
    }
    else if (mCurrent.type == INT ) {
        mCurrentMaterial->alpha=mCurrent.value.intValue;
    }
    else {
        //TODO explosion
    }
    next();
}

void MtlParser::parseTr() {
    next();
    if (mCurrent.type == FLOAT ) {
        mCurrentMaterial->alpha=1.0-mCurrent.value.floatValue;
    }
    else if (mCurrent.type == INT ) {
        mCurrentMaterial->alpha=1-mCurrent.value.intValue;
    }
    else {
        //TODO explosion
    }
    next();
}
void MtlParser::parseNs() {
    next();
    if (mCurrent.type == FLOAT ) {
        mCurrentMaterial->shininess=mCurrent.value.floatValue;
    }
    else if (mCurrent.type == INT ) {
        mCurrentMaterial->shininess=mCurrent.value.intValue;
    }
    else {
        //TODO explosion
    }
    next();
}

void MtlParser::parseMap_Kd() {
    next();
    std::string filename="";
    while (mCurrent.type != NEWLINE && mCurrent.type != END) {
        filename += mCurrent.identifier;   // accumule tous les tokens
        next();
    }

   mCurrentMaterial->ColorTextureID = mScene->getTextureId(mDir + filename);
    next();
}

glm::vec3 MtlParser::parseVec3() {
    glm::vec3 v=glm::vec3(0.);
    int coord=0;
    next();
    while (mCurrent.type != NEWLINE && mCurrent.type != END ){//TODO les commentaires en fin de ligne enculent cela
        if (coord < 3 ) {
            if (mCurrent.type==FLOAT) {
                v[coord]= mCurrent.value.floatValue;
            }
            else if (mCurrent.type==INT) {
                v[coord]= mCurrent.value.intValue;
            }
            else {
                //error("Expected a number"); TODO erreurs
            }
        }
        else {
            //notEnoughComponentsError(4);
        }
        coord++;
        next();
    }

    return v;
}