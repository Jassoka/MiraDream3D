//
// Created by Mathis Pean on 19/06/2026.
//

#ifndef MIRADREAM3D_MTLPARSER_HPP
#define MIRADREAM3D_MTLPARSER_HPP

#include "objParsing.hpp"
#include "model/texture_types.hpp"

class MtlParser {
public:
    MtlParser(const std::string &file,Scene* scene);
    static void parse(const std::string &file,Scene* scene);
private:

    void parseImpl();

    ObjLexer mLexer;
    ObjToken mCurrent;
    Material* mCurrentMaterial=nullptr;
    Scene* mScene;
    std::string mSrc;

    void parseNewmtl();
    void parseKs();
    void parseKd();
    void parseKa();
    void parseD();
    void parseTr();
    void parseNs();
    glm::vec3 parseVec3();
    void next(){mCurrent=mLexer.next();}



};


#endif //MIRADREAM3D_MTLPARSER_HPP
