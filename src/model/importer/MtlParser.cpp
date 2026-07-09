//
// Created by Mathis Pean on 19/06/2026.
//

#include "MtlParser.hpp"
#include <filesystem>

#include "glm/vec3.hpp"
#include "model/Scene.h"


void MtlParser::parseImpl() {
    next();
    while (mCurrent.type != END ) {
        if (mCurrent.type != IDENTIFIER) {
            if (mCurrent.type!=NEWLINE)
                throwError(ParserMessages::UnexpectedToken);
        }
        else
        {
            if (mCurrent.identifier=="newmtl")
                parseNewmtl();
            else if (mCurrentMaterial==nullptr)
                throwError("No material selected");
            else if (mCurrent.identifier=="Ks")
                parseKs();
            else if (mCurrent.identifier=="Ka")
                parseKa();
            else if (mCurrent.identifier=="Kd")
                parseKd();
            else if (mCurrent.identifier=="d")
                parseD();
            else if (mCurrent.identifier=="Tr")
                parseTr();
            else if (mCurrent.identifier=="Ns")
                parseNs();
            else if (mCurrent.identifier=="map_Kd")
                parseMap_Kd();
            else {
                while (mCurrent.type !=NEWLINE) {
                    next();
                }
            }
        }
        expectEOL();
    }
}



void MtlParser::parseNewmtl() {
    next();
    if (mCurrent.type == IDENTIFIER)
    {
        std::string name = parseName();
        mCurrentMaterial=mScene->giveNewMaterial(name);
    }
}

void MtlParser::parseKs() {
    next();
    mCurrentMaterial->Ks=parseVec3();
}

void MtlParser::parseKd() {
    next();
    mCurrentMaterial->Kd=parseVec3();
}
void MtlParser::parseKa() {
    next();
    mCurrentMaterial->Ka=parseVec3();
}
void MtlParser::parseD() {
    next();
    mCurrentMaterial->alpha=parseNumber();
}

void MtlParser::parseTr() {
    next();
    mCurrentMaterial->alpha=parseNumber();
}
void MtlParser::parseNs() {
    next();
    mCurrentMaterial->shininess=parseNumber();
}

void MtlParser::parseMap_Kd() {
    next();
    const std::string filename = parseName();
    const std::string path = mDir + filename;
    const int32_t colorTextureID = mScene->getTextureId(path);
    if (colorTextureID == -1)
    {
        throwWarning(ParserMessages::FileNotFound, path);
        mCurrentMaterial->ColorTextureID = DEFAULT_TEXTURE;
    }
    else
    {
        mCurrentMaterial->ColorTextureID = colorTextureID;
    }
    next();
}