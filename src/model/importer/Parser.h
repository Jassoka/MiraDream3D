//
// Created by jassoka on 7/4/26.
//

#ifndef MIRADREAM3D_MODELPARSER_H
#define MIRADREAM3D_MODELPARSER_H
#include <string>

#include "Lexer.h"

class Mesh;
class Node;
class Scene;
struct MeshBuildFlags;
struct MeshBuildData;


class ParserException : public std::runtime_error {
public:
    ParserException(const std::string& msg, const int line, const int col)
        : std::runtime_error("[Parser l." + std::to_string(line) +
                       " c." + std::to_string(col) + "] " + msg) {}
};

class Parser
{
public:
    Parser(const std::string &file,Scene* scene);
    ~Parser() = default;
    static void parse(const std::string &file,Scene* scene);
protected:
    void virtual parseImpl() = 0;
    void next();
    void throwError(const std::string &msg) const;

    LexerToken mCurrent;
    Lexer mLexer;
    Scene* mScene;
    std::string mDir;

    MeshBuildFlags *mMeshBuildFlags;
    MeshBuildData *mMeshBuildData;
};


#endif //MIRADREAM3D_MODELPARSER_H
