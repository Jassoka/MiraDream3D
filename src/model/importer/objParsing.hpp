//
// Created by Mathis Pean on 09/06/2026.
//

#ifndef MIRADREAM3D_OBJPARSING_HPP
#define MIRADREAM3D_OBJPARSING_HPP
#include <string>


enum ObjTokenType {
    IDENTIFIER,
    INT,
    FLOAT,
    SLASH,
    NEWLINE,
    SPACE,
    END,
    UNKNOWN
};
struct ObjToken {
    ObjTokenType type;
    union value{
        std::string* identifier;
        int intValue;
        float floatValue;
    };

};
class ObjLexer {
public:
    ObjLexer(const std::string &file):mSrc(file){};
    ObjToken next();

private:
    void advance();
    ObjToken readIdentifier();
    ObjToken readNumber();
    void readSpace();
    void skipLine();
    ObjToken errorToken();
    uint32_t mLin=0;
    uint32_t mCol=0;
    uint32_t mPos=0;
    std::string mSrc;

};
class ObjParser {

};


#endif //MIRADREAM3D_OBJPARSING_HPP
