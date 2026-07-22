//
// Created by jassoka on 7/4/26.
//

#include "model/parser/Lexer.h"

#include <cmath>

//accepte 1 à 9, ., - ;
bool isNumber(const char c) {
    return '0'<=c && c<='9' ;
}
//accepte les lettres maj/min et le _
bool isLetter(const char c) {
    return ('a'<=c && c<='z') || ('A'<=c && c<='Z')  ;
}

LexerToken Lexer::next() {
    readSpace();
    if (mPos>=mSrc.size()) {
        return LexerToken{.type=END};
    }
    const char c = mSrc[mPos];
    if (c=='\n') {
        skipLine();
        return(LexerToken{.type=NEWLINE});
    }
    if (c=='/') {
        mPos++; mCol++;
        return LexerToken{.type=SLASH, .identifier = "/"};
    }
    if (c=='#') {
        skipLine();
        return LexerToken{NEWLINE};
    }
    if (isLetter(c)|| c=='_'){return readIdentifier();}
    if (isNumber(c) || c=='-' || c=='.'||c=='+' ){return readNumber();}

    //std::cout << "error char :" << c << std::endl;
    throwError("Unknown grammar");
}


LexerToken Lexer::readIdentifier() {

    LexerToken token;
    token.type=IDENTIFIER;

    uint32_t stringPos=0;
    while (mPos < mSrc.size() &&
       (isLetter(mSrc[mPos]) || isNumber(mSrc[mPos]) || mSrc[mPos]=='_'))
    {
        token.identifier += mSrc[mPos];
        ++mPos;
        ++mCol;
    }
    return token;
}
//voir grammaire discord
LexerToken Lexer::readNumber(){
    char c=mSrc[mPos];
    LexerToken token;

    bool isValid=false;
    bool isFloat=false;
    bool isNegative=false;
    bool hasSign=false;
    bool hasExp=false;
    bool isExpNeg=false;
    int exponent=0;
    int distanceFromComma=1;

    float number=0;
    while (mPos<mSrc.size() && (isNumber(c) || c=='E' || c=='e' || c=='-'||c=='+'||c=='.')) {
        token.identifier+=c;
        if (c=='.') {
            isValid=false;
            if (isFloat) {
                break;
            }
            isFloat = true;
        }
        else if (c=='-') {
            isValid=false;
            if (hasSign) {
                break;
            }
            if (hasExp) {
                isExpNeg=true;
            }
            else {
                isNegative=true;
            }
            hasSign=true;

        }
        else if (c=='+') {
            isValid=false;
            if (hasSign) {
                break;
            }
            hasSign=true;
        }
        else if (c=='e' || c=='E') {
            isValid=false;
            if (hasExp) {
                break;
            }
            isFloat=true;
            hasSign=false;
            hasExp=true;
        }
        else {
            isValid=true;
            const int chiffre=c-'0';
            if (hasExp) {
                exponent*=10;
                exponent+=chiffre;
            }
            else if (isFloat) {
                number+=chiffre/std::pow(10,distanceFromComma);
                distanceFromComma++;
            }
            else {
                number*=10;
                number+=chiffre;
            }
        }


        mPos++;
        mCol++;
        c=mSrc[mPos];
    }
    if (!isValid) {

        while (mPos<mSrc.size() && c!='\n') {
            token.identifier+=c;
            mPos++;
            mCol++;
            c=mSrc[mPos];
        }

        token.type=IDENTIFIER;
        return(token);

        //error("Not a number ");
    }
    if (hasExp) {
        if (isExpNeg) exponent=-exponent;
        number*=std::pow(10,exponent);
    }
    if (isNegative) number=-number;
    if (isFloat) {
        token.value.floatValue=number;
        token.type=FLOAT;
    }
    else {
        token.value.intValue=static_cast<int>(number);
        token.type=INT;
    }
    return token;
}


void Lexer::readSpace() {
    while (mPos < mSrc.size() &&
           std::isspace(static_cast<unsigned char>(mSrc[mPos])) &&
           mSrc[mPos] != '\n') {
        mPos++;
        mCol++;
           }
}

void Lexer::skipLine() {
    while (mPos < mSrc.size() && mSrc[mPos] != '\n')
        mPos++;
    mLin++;
    mCol=0;
    mPos++;
}


void Lexer::throwError(const std::string &msg) const {
    throw LexerException(msg, mFilePath,mLin,mCol);
}

