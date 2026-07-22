//
// Created by jassoka on 7/4/26.
//

#ifndef MIRADREAM3D_MODELLEXER_H
#define MIRADREAM3D_MODELLEXER_H
#include <cstdint>
#include <stdexcept>
#include <string>

#include "util/parser_utils.hpp"

enum LexerTokenType {
    IDENTIFIER,
    INT,
    FLOAT,
    SLASH,
    NEWLINE,
    SPACE,
    END,
    UNKNOWN
};
struct LexerToken {
    LexerTokenType type;
    union Value{
        int intValue;
        float floatValue;
    } value;
    std::string identifier="";
};

class LexerException : public std::runtime_error {
public:
    LexerException(const std::string& msg, const std::string &fileLocation, const int line, const int col)
        : std::runtime_error(parserMessageFormat(ERROR, "Lexer", fileLocation , msg, line, col)) {}
};

class Lexer
{
public:
    explicit Lexer(const std::string &file, const std::string &path):mSrc(file), mFilePath(path){};
    [[noreturn]] void throwError(const std::string &msg) const;
    LexerToken next();
    uint32_t getLine() const {return mLin;}
    uint32_t getCol() const {return mCol;}
    const std::string &getFilePath() const {return mFilePath;}
private:
    LexerToken readIdentifier();
    LexerToken readNumber();
    void readSpace();
    void skipLine();
    uint32_t mLin=0;
    uint32_t mCol=0;
    uint32_t mPos=0;
    std::string mSrc;
    std::string mFilePath;
};


#endif //MIRADREAM3D_MODELLEXER_H
