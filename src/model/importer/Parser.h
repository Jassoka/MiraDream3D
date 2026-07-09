//
// Created by jassoka on 7/4/26.
//

#ifndef MIRADREAM3D_MODELPARSER_H
#define MIRADREAM3D_MODELPARSER_H
#include <sstream>
#include <string>

#include "Lexer.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class Mesh;
class Node;
class Scene;
struct MeshBuildFlags;
struct MeshBuildData;


class ParserException : public std::runtime_error {
public:
    ParserException(const std::string& msg,const std::string &fileLocation, const int line, const int col)
        : std::runtime_error(parserMessageFormat(ERROR, "Parser", fileLocation ,msg, line, col)) {}
};


namespace ParserMessages
{
    inline const std::string ExpectedNumber = "expected a number";
    inline const std::string ExpectedInt = "expected an integer";
    inline const std::string UnexpectedIdentifier = "unexpected identifier";
    inline const std::string UnexpectedToken = "unexpected token";
    inline const std::string ExpectedEOL = "expected end of line";
    inline const std::string FileNotFound = "file not found";
    inline const std::string VectorTooLarge = "vector has too many coordinates";

}

class Parser
{
public:
    Parser(const std::string &file, Scene* scene, std::ostringstream &warningStream);
    ~Parser() = default;
protected:
    template <typename ParserT>
    static void parseTemplate(const std::string &file, Scene* scene, std::ostringstream& warnings)
    {
        auto instance = ParserT(file, scene, warnings);
        instance.initFlags();
        instance.parseImpl();
    }
    void virtual initFlags() = 0;
    void virtual parseImpl() = 0;

    void next();
    [[noreturn]] void throwError(const std::string &msg, const std::string &detail = "") const;
    void throwWarning(const std::string &msg, const std::string &detail);

    glm::vec2 parseVec2() { return parseVec<2>(); }
    glm::vec3 parseVec3() { return parseVec<3>(); }
    float parseNumber();
    int parseInt();
    inline std::string parseName()
    {
        std::string name="";
        while (mCurrent.type != NEWLINE && mCurrent.type != END) {
            name += mCurrent.identifier;   // accumule tous les tokens
            next();
        }
        return name;
    }

    void expectEOL();
    void expectToken(LexerTokenType tokenType, const std::string& msg = ParserMessages::UnexpectedToken);

    LexerToken mCurrent;
    Lexer mLexer;
    Scene* mScene;
    std::string mDir;

    MeshBuildFlags *mMeshBuildFlags;
    MeshBuildData *mMeshBuildData;

    bool mWarningThrown;
    std::ostringstream &mWarnings;
private:
    template <int dimension>
    glm::vec<dimension, float> parseVec();
};


#endif //MIRADREAM3D_MODELPARSER_H
