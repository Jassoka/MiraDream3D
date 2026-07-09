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
    inline static auto parse =
        &Parser::parseTemplate<MtlParser>;
    friend class Parser;
private:
    using Parser::Parser;
    void parseImpl() override;
    void initFlags() override {}
    void parseNewmtl();
    void parseKs();
    void parseKd();
    void parseKa();
    void parseD();
    void parseTr();
    void parseNs();
    void parseMap_Kd();

    Material* mCurrentMaterial=nullptr;
};


#endif //MIRADREAM3D_MTLPARSER_HPP
