//
// Created by jassoka on 7/22/26.
//

#ifndef MIRADREAM3D_SHADERPARSER_H
#define MIRADREAM3D_SHADERPARSER_H
#include "../parser/Parser.h"
#include "types.h"


class ShaderParser final: public Parser
{
public:
    /**
     * @brief Parses a shader and returns its new contents as a string
     * @param fileContent String containing the code
     * @param path Path of shader (for includes)
     * @param warnings Output for parser warnings
     * @param defines Vector of compile time define values
     */
    static std::string parse(const std::string& fileContent,
                      const std::string& path,
                      std::ostringstream& warnings,
                      const std::vector<DefineField> &defines)
    {
        auto instance = ShaderParser(fileContent, path, warnings, defines);
        instance.executeParser();
        return instance.mCode;
    }
protected:
    ShaderParser(const std::string& fileContent,
                      const std::string& path,
                      std::ostringstream& warnings,
                      const std::vector<DefineField> &defines);
    void parseImpl() override;
private:
    void parseVersion();
    void addDefines();
    std::vector<DefineField> mDefineFields;
    /** @brief Shader output code */
    std::string mCode;
};

#endif //MIRADREAM3D_SHADERPARSER_H
