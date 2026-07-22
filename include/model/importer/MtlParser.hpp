//
// Created by Mathis Pean on 19/06/2026.
//

#ifndef MIRADREAM3D_MTLPARSER_HPP
#define MIRADREAM3D_MTLPARSER_HPP

#include "ImportParser.h"
#include "../parser/Parser.h"
#include "model/texture_types.hpp"

class MtlParser final: public ImportParser {
public:
    //friend class Parser;
    static void parse(const std::string &file, std::ostringstream& warnings, SceneImport &sceneOutput)
    {
        auto instance = MtlParser(file, warnings, sceneOutput);
        instance.executeParser();
    }
protected:
    using ImportParser::ImportParser;
    void parseImpl() override;
    void initFlags() override {}
private:
    void parseNewmtl();
    void parseKs();
    void parseKd();
    void parseKa();
    void parseD();
    void parseTr();
    void parseNs();
    void parseMap_Kd();

    int32_t mCurrentMaterialID = -1;
};


#endif //MIRADREAM3D_MTLPARSER_HPP
