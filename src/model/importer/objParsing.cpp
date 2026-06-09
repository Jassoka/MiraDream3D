//
// Created by Mathis Pean on 09/06/2026.
//

#include "objParsing.hpp"

#include <iostream>


//accepte 1 à 9, ., - ;
bool isNumber(char &c) {
    return '0'<=c && c<='9' ;
}
//accepte les lettres maj/min et le _
bool isLetter(char &c) {
    return 'a'<=c && c<='b'|| ('A'<=c && c<='B')  ;
}


ObjToken ObjLexer::next() {
    readSpace();
    if (mPos>=mSrc.size()) {
        return make(END,"");
    }
    char c=mSrc[mPos];

    if (c=='\n') {
        return(make(NEWLINE,""));
    }
    if (c=='/') {
        return make(SLASH,"");
    }
    if (c=='#') {
        skipLine();
        return next();
    }
    if (isLetter(c)){return readIdentifier();}
    if (isNumber(c) || c=='_' || c=='.' ){return readNumber();}
    std::cout <<"mot clef inconnu dans le Lexer"<<std::endl;
    return (make(UNKNOWN,""));
}
ObjToken ObjLexer::readIdentifier() {
    char c=mSrc[mPos];
    std::string value="";
    while (mPos<mSrc.size() && (isLetter(c) || isNumber(c) || c=='_')) {
        value+=c;
        mPos++;
        mCol++;
        c=mSrc[mPos];
    }
    return make(IDENTIFIER,value);
}
ObjToken ObjLexer::readNumber(){
    char c=mSrc[mPos];
    std::string value="";
    while (mPos<mSrc.size() && isNumber(c)) {
        value+=c;
        if (c=='.') {
            bool comma = true;
        }
        mPos++;
        mCol++;
        c=mSrc[mPos];

    }
    return make(type,value);
}

void ObjLexer::readSpace(){
    char c = mSrc[mPos];
    // retour a la ligne soit deja lu, soit au debut du fichier
    if (c=='\n') {
        skipLine();
    }

    //on passe les espaces
    while (mPos<mSrc.size() && c==' ') {
        mPos++;
        mCol++;
        c=mSrc[mPos];
    }
}

ObjToken ObjLexer::make(ObjTokenType type,std::string value) {
    return ObjToken{type,value};
}

void ObjLexer::skipLine() {
    mLin++;
    mCol=0;
    mPos++;
}

ObjToken ObjLexer::errorToken() {

}