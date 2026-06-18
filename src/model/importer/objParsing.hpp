//
// Created by Mathis Pean on 09/06/2026.
//

#ifndef MIRADREAM3D_OBJPARSING_HPP
#define MIRADREAM3D_OBJPARSING_HPP
#include <string>
#include <unordered_map>

#include "util/file_funcs.hpp"
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
    uint32_t getLine() const {return mLin;}
    uint32_t getCol() const {return mCol;}
private:
    ObjToken readIdentifier();
    ObjToken readNumber();
    void readSpace();
    void skipLine();
    void error(const std::string &msg) const;
    uint32_t mLin=0;
    uint32_t mCol=0;
    uint32_t mPos=0;
    std::string mSrc;

};
class ObjParser {
public:

    ObjParser(const std::string &file,Scene* scene):mLexer(ObjLexer( readFileToString(file))),mScene(scene) {};
    void parse();
private:

    ObjToken mCurrent;
    ObjLexer mLexer;
    Scene* mScene;
    Node* mCurrentNode=nullptr;
    Mesh* mCurrentMesh=nullptr;
    Node* mDefaultMeshNode=nullptr;
    uint32_t mCurrentMeshOriginVId=0;
    bool mCurrentMeshHasNormals=true;
    bool mCurrentMeshHasUVCoords=true;
    uint8_t mCurrentSmoothGroup=0;
    std::unordered_map<uint8_t,uint8_t> mCurrentMeshSmoothGroupsMap;
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
    void parseS();
    void parseUsemtl();
    void parseMtllib();
    void parseL();

    void error(const std::string &msg) const;
    void notEnoughComponentsError(int i) const;
    void removeDefaultMesh();
    void createMesh(std::string name);

};

class ObjException : public std::runtime_error {
public:
    ObjException(const std::string& msg)
        : std::runtime_error(msg) {}

};


class ObjLexerException : public ObjException {
public:
    ObjLexerException(const std::string& msg, int line, int col)
        : ObjException("[Lexer l." + std::to_string(line) +
                       " c." + std::to_string(col) + "] " + msg) {}
};

class ObjParserException : public ObjException {
public:
    ObjParserException(const std::string& msg, int line, int col)
        : ObjException("[Parser l." + std::to_string(line) +
                       " c." + std::to_string(col) + "] " + msg) {}
};





#endif //MIRADREAM3D_OBJPARSING_HPP
