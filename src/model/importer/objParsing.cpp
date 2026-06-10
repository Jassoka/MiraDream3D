//
// Created by Mathis Pean on 09/06/2026.
//

#include "objParsing.hpp"

#include <array>
#include <iostream>


//accepte 1 à 9, ., - ;
bool isNumber(char &c) {
    return '0'<=c && c<='9' ;
}
//accepte les lettres maj/min et le _
bool isLetter(char &c) {
    return ('a'<=c && c<='z') || ('A'<=c && c<='Z')  ;
}


ObjToken ObjLexer::next() {
    readSpace();
    if (mPos>=mSrc.size()) {
        return ObjToken{.type=END};
    }
    char c=mSrc[mPos];

    if (c=='\n') {
        skipLine();
        return(ObjToken{.type=NEWLINE});
    }
    if (c=='/') {
        mPos++; mCol++;
        return ObjToken{.type=SLASH};
    }
    if (c=='#') {
        skipLine();
        return next();
    }
    if (isLetter(c)|| c=='_'){return readIdentifier();}
    if (isNumber(c) || c=='-' || c=='.'||c=='+' ){return readNumber();}

    return (errorToken());
}
ObjToken ObjLexer::readIdentifier() {
    char c=mSrc[mPos];

    ObjToken token;
    token.type=IDENTIFIER;

    uint32_t stringPos=0;
    while (mPos<mSrc.size() && (isLetter(c) || isNumber(c) || c=='_')) {
        token.identifier+=c;
        mPos++;
        mCol++;
        c=mSrc[mPos];
    }
    return token;
}
//voir grammaire discord
ObjToken ObjLexer::readNumber(){
    char c=mSrc[mPos];
    ObjToken token;

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
    if (!isValid) return errorToken();
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

void ObjLexer::readSpace(){
    char c = mSrc[mPos];
    while (mPos<mSrc.size() && c==' ') {
        mPos++;
        mCol++;
        c=mSrc[mPos];
    }
}

void ObjLexer::skipLine() {
    while (mPos < mSrc.size() && mSrc[mPos] != '\n')
        mPos++;
    mLin++;
    mCol=0;
    mPos++;
}

ObjToken ObjLexer::errorToken() {
    std::cout <<"mot clef inconnu dans le Lexer"<<std::endl;
    return ObjToken{.type=UNKNOWN};
}

void ObjParser::parseV() {
    glm::vec3 v;
    auto expectedTokenType= INT;
    int coord=0;
    while (mCurrent.type != NEWLINE && mCurrent.type != END ){//TODO les commentaires en fin de ligne enculent cela
        next();
        if (mCurrent.type!=expectedTokenType) {
            break;
        }
        if (mCurrent.type == FLOAT) {
            v[coord]=mCurrent.value.floatValue;
            expectedTokenType=SLASH;
            coord++;
        }
        else expectedTokenType=FLOAT;
    }
    if (coord!=3) {
        return;
    }//TODO explosion

    mV.push_back(v);
    mCurrentMesh->addGeometricVertex(GeometricVertex {});
}

void ObjParser::parseVN() {
    glm::vec3 v;
    auto expectedTokenType= INT;
    int coord=0;
    while (mCurrent.type != NEWLINE && mCurrent.type != END ){//TODO les commentaires en fin de ligne enculent cela
        next();
        if (mCurrent.type!=expectedTokenType) {
            break;
        }
        if (mCurrent.type == FLOAT) {
            v[coord]=mCurrent.value.floatValue;
            expectedTokenType=SLASH;
            coord++;
        }
        else expectedTokenType=FLOAT;
    }
    if (coord!=3) {
        return;
    }//TODO explosion

    mVN.push_back(v);
}

void ObjParser::parseVT() {
    glm::vec3 v=glm::vec3(0);
    auto expectedTokenType= INT;
    int coord=0;
    while (mCurrent.type != NEWLINE && mCurrent.type != END ){//TODO les commentaires en fin de ligne enculent cela
        next();
        if (mCurrent.type!=expectedTokenType) {
            break;
        }
        if (mCurrent.type == FLOAT) {
            v[coord]=mCurrent.value.floatValue;
            expectedTokenType=SLASH;
            coord++;
        }
        else expectedTokenType=FLOAT;
    }
    if (coord<3&&coord>4) {
        return;
    }//TODO explosion

    mVT.push_back(v);
}

void ObjParser::parseO() {
    next();
    std::string name=" ";
    if (mCurrent.type==IDENTIFIER) {
        name=mCurrent.identifier;
    }

    mCurrentNode = new Node(name);    //TODO liberer
    mParentNode->addChild(mCurrentNode);
}
void ObjParser::parseF(Mesh &parentMesh) {
    uint nVertex=0;
    uint nComposante=0;
    next();
    int v=-1;
    int vn=-1;
    int vt=-1;


    while (mCurrent.type != NEWLINE && mCurrent.type != END ) {

        if (mCurrent.type!=IDENTIFIER) {
            break;
        }

        next();

        mCurrentMesh->addVertex();
    }

    switch (nVertex) {
        case(3):
            parentMesh.addTriangle( );
            break;
        case(4):
            parentMesh.addQuad();
            break;
        default:
            return;//TODO explosion
            break;
    }
}
