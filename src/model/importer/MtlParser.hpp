//
// Created by Mathis Pean on 19/06/2026.
//

#ifndef MIRADREAM3D_MTLPARSER_HPP
#define MIRADREAM3D_MTLPARSER_HPP

#include "Parser.h"
#include "glm/fwd.hpp"
#include "model/texture_types.hpp"

class MtlParser: Parser {
public:
    MtlParser(const std::string &file,Scene* scene);

    static void parse(const std::string &file,Scene* scene);
private:
    void parseImpl() override;
    void parseNewmtl();
    void parseKs();
    void parseKd();
    void parseKa();
    void parseD();
    void parseTr();
    void parseNs();
    void parseMap_Kd();
    glm::vec3 parseVec3();

    Material* mCurrentMaterial=nullptr;
};


#endif //MIRADREAM3D_MTLPARSER_HPP
