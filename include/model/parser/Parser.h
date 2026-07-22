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
#include "model/SceneImport.h"

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
    inline const std::string MaterialNotFound = "material not found";

}

/** @brief Template for parser class */
class Parser
{
public:
    Parser(const std::string &file, std::ostringstream &warningStream);
    virtual ~Parser() = default;
protected:
    virtual void executeParser();
    void virtual parseImpl() = 0;

    void next();
    [[noreturn]] void throwError(const std::string &msg, const std::string &detail = "") const;
    void throwWarning(const std::string &msg, const std::string &detail = "");

    float parseNumber();
    int parseInt();
    std::string parseString();
    /** @brief Skips whole line */
    void skipLine();
    /**
     * @brief Throws error if next token is not an end of line or end of file
     * Skips next token
     */
    void expectEOL();

    /**
     * @brief Throws error if next token is unexpected, skips next token
     * @param tokenType Expected token
     * @param msg Error message
     */
    void expectToken(LexerTokenType tokenType, const std::string& msg = ParserMessages::UnexpectedToken);

    LexerToken mCurrent;
    Lexer mLexer;
    std::string mDir;

    bool mWarningThrown;
    std::ostringstream &mWarnings;
};


#endif //MIRADREAM3D_MODELPARSER_H
