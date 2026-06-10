//
// Created by Mathis Pean on 09/06/2026.
//

#ifndef MIRADREAM3D_OBJPARSING_HPP
#define MIRADREAM3D_OBJPARSING_HPP
#include <string>

#include "model/Node.h"
#define MAX_OBJ_NAME_SIZE 10

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

    ObjParser(const std::string &file,Node* parentNode):mLexer(ObjLexer(file)),mParentNode(parentNode){};

private:

    void execLexer();
    void parse();

    ObjToken mCurrent;
    ObjLexer mLexer;
    Node* mParentNode;
    Node* mCurrentNode=nullptr;
    Mesh* mCurrentMesh=nullptr;
    uint32_t mCurrentMeshOrginVId=0;
    std::vector<glm::vec3> mV;
    std::vector<glm::vec3> mVN;
    std::vector<glm::vec3> mVT;
    std::vector<uint32_t> mNoNormal;
    std::vector<uint32_t> mNoUv;

    void next(){mCurrent=mLexer.next();};

    void parseV();
    void parseVN();
    void parseVT();
    void parseF(Mesh &parentMesh);
    void parseO();
    void parseG();
    void parseUsemtl();
    void parseMtllib();
    void parseL(Mesh &parentMesh);


};


#endif //MIRADREAM3D_OBJPARSING_HPP
