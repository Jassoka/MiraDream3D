//
// Created by Mathis Pean on 19/06/2026.
//

#include "model/importer/MtlParser.hpp"

#include <filesystem>

#include "glm/vec3.hpp"
#include "model/Scene.h"
#include "model/importer/ObjLexer.h"
#include "util/file_funcs.hpp"

MtlParser::MtlParser(const std::string &path, std::ostringstream& warnings, SceneImport &sceneOutput):
ImportParser(
    new ObjLexer(readFileToString(path), path),
    path,
    warnings,
    sceneOutput)
{}

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
            else
            {
                if (mCurrentMaterialID < 0) // Aucun matériau selectionné
                    skipLine();
                else
                {
                    if (mCurrent.identifier=="Ks")
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
                    else
                        skipLine();
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
        const std::string name = parseString();
        mCurrentMaterialID=mSceneImport.newLocalMaterial(name);
    }
}

void MtlParser::parseKs() {
    next();
    mSceneImport.getLocalMaterial(mCurrentMaterialID).Ks=parseVec3();
}

void MtlParser::parseKd() {
    next();
    mSceneImport.getLocalMaterial(mCurrentMaterialID).Kd=parseVec3();
}
void MtlParser::parseKa() {
    next();
    mSceneImport.getLocalMaterial(mCurrentMaterialID).Ka=parseVec3();
}
void MtlParser::parseD() {
    next();
    mSceneImport.getLocalMaterial(mCurrentMaterialID).alpha=parseNumber();
}

void MtlParser::parseTr() {
    next();
    mSceneImport.getLocalMaterial(mCurrentMaterialID).alpha= 1.0 - parseNumber();
}
void MtlParser::parseNs() {
    next();
    mSceneImport.getLocalMaterial(mCurrentMaterialID).shininess=parseNumber();
}

void MtlParser::parseMap_Kd() {
    next();
    const std::string filename = parseString();
    const std::string path = mDir + filename;
    const uint32_t colorTextureID = mSceneImport.getLocalTextureID(path);
    mSceneImport.getLocalMaterial(mCurrentMaterialID).ColorTextureID = colorTextureID;
}