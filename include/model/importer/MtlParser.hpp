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
    friend class Parser;
    static void parse(const std::string &file, SceneImport &sceneOutput, std::ostringstream& warnings)
    {
        auto instance = MtlParser(file, sceneOutput, warnings);
        instance.executeParser();
    }
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

    int32_t mCurrentMaterialID;
};


#endif //MIRADREAM3D_MTLPARSER_HPP
