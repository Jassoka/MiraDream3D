//
// Created by jassoka on 7/4/26.
//

#ifndef MIRADREAM3D_MODELLEXER_H
#define MIRADREAM3D_MODELLEXER_H
#include <cstdint>
#include <stdexcept>
#include <string>

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
    LexerException(const std::string& msg, const int line, const int col)
        : std::runtime_error("[Lexer l." + std::to_string(line) +
                       " c." + std::to_string(col) + "] " + msg) {}
};

class Lexer
{
public:
    explicit Lexer(const std::string &file):mSrc(file){};
    [[noreturn]] void throwError(const std::string &msg) const;
    LexerToken next();
    uint32_t getLine() const {return mLin;}
    uint32_t getCol() const {return mCol;}
private:
    LexerToken readIdentifier();
    LexerToken readNumber();
    void readSpace();
    void skipLine();
    uint32_t mLin=0;
    uint32_t mCol=0;
    uint32_t mPos=0;
    std::string mSrc;
};


#endif //MIRADREAM3D_MODELLEXER_H
