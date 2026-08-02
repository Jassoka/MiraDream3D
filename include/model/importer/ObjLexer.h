//
// Created by jassoka on 7/24/26.
//

#ifndef MIRADREAM3D_OBJLEXER_H
#define MIRADREAM3D_OBJLEXER_H
#include "model/parser/Lexer.h"


class ObjLexer: public Lexer
{
public:
    using Lexer::Lexer;
    LexerToken next() override {
        readSpace();
        if (mPos>=mSrc.size()) {
            return LexerToken{.type=END};
        }
        const char c = mSrc[mPos];
        if (c=='\n') {
            skipLine();
            return(LexerToken{.type=NEWLINE});
        }
        if (c=='/' || c=='\\') {
            mPos++; mCol++;
            return LexerToken{.type=SLASH, .identifier = "/"};
        }
        if (c=='#') {
            skipLine();
            return LexerToken{.type = NEWLINE};
        }
        if (c=='(' || c == ')' || c == ':')
        {
            mPos++; mCol++;
            return LexerToken{.type = SYMBOL, .identifier = std::string(1, c)};
        }
        if (isLetter(c)|| c=='_'){return readIdentifier();}

        if (c == '.')
        {
            if (isNumber(mSrc[mPos+1])) return readNumber();
            mPos++; mCol++;
            return LexerToken{.type = SYMBOL, .identifier = std::string(1, c)};
        }
        if (isNumber(c) || c=='-' || c=='+' ){return readNumber();}

        throwError("unknown grammar");
    }
};
#endif //MIRADREAM3D_OBJLEXER_H
