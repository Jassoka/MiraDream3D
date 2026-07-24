//
// Created by jassoka on 7/24/26.
//

#ifndef MIRADREAM3D_SHADERLEXER_H
#define MIRADREAM3D_SHADERLEXER_H
#include "model/parser/Lexer.h"

class GLSLLexer: public Lexer
{
    using Lexer::Lexer;
    LexerToken next() override
    {
        readSpace();
        if (mPos>=mSrc.size()) {
            return LexerToken{.type=END};
        }
        const char c = mSrc[mPos];
        if (c=='\n') {
            skipLine();
            return(LexerToken{.type=NEWLINE});
        }
        if (c == '#')
        {
            mPos++; mCol++;
            const auto directive = readIdentifier();
            return(LexerToken{.type=GLSL_DIRECTIVE, .identifier = "#" + directive.identifier});
        }
        if (isLetter(c)|| c=='_'){return readIdentifier();}
        if (isNumber(c) || c=='-' || c=='.'||c=='+' ){return readNumber();}
        mPos++; mCol++;
        return LexerToken{.type = SYMBOL, .identifier = std::string(1, c)};
    }
};

#endif //MIRADREAM3D_SHADERLEXER_H
