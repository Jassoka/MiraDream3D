//
// Created by Mathis Pean on 09/06/2026.
//

#ifndef MIRADREAM3D_OBJPARSING_HPP
#define MIRADREAM3D_OBJPARSING_HPP
#include <string>

#include "file_funcs.hpp"
#include "glm/vec2.hpp"
#include "model/Node.h"
#define MAX_OBJ_NAME_SIZE 10

class Scene;

enum ObjTokenType {
    IDENTIFIER,
    INT,
    FLOAT,
    SLASH,
    NEWLINE,
    SPACE,
    END,
    UNKNOWN
};
struct ObjToken {
    ObjTokenType type;
    union Value{
        int intValue;
        float floatValue;
    } value;
    std::string identifier="";
};
class ObjLexer {
public:
    ObjLexer(const std::string &file):mSrc(file){};
    ObjToken next();

private:
    ObjToken readIdentifier();
    ObjToken readNumber();
    void readSpace();
    void skipLine();
    ObjToken errorToken();
    uint32_t mLin=0;
    uint32_t mCol=0;
    uint32_t mPos=0;
    std::string mSrc;

};
class ObjParser {
public:

    ObjParser(const std::string &file,Scene* scene):mLexer(ObjLexer( readFileToString(file))),mScene(scene){};
    void parse();
private:
    void execParser();


    ObjToken mCurrent;
    ObjLexer mLexer;
    Scene* mScene;
    Node* mCurrentNode=nullptr;
    Mesh* mCurrentMesh=nullptr;
    Node* mDefaultMeshNode=nullptr;
    uint32_t mCurrentMeshOriginVId=0;
    bool mCurrentMeshHasNormals=true;
    bool mCurrentMeshHasUVCoords=true;
    std::vector<std::vector<uint32_t>> mCurrentMeshFacePerGeometricVertex;
    std::unordered_map<uint32_t,uint32_t> mCurrentMeshGeometricVerticesMap;
    std::vector<glm::vec3> mV;
    std::vector<glm::vec3> mVN;
    std::vector<glm::vec2> mVT;
    std::vector<uint32_t> mNoNormal;
    std::vector<uint32_t> mNoUv;

    void next(){mCurrent=mLexer.next();};
    void finishMesh();
    void parseV();
    void parseVN();
    void parseVT();
    void parseF();
    void parseO();
    void parseG();
    void parseL();
    void parseUsemtl();
    void parseMtllib();


    void removeDefaultMesh();


};


#endif //MIRADREAM3D_OBJPARSING_HPP
