//
// Created by jassoka on 7/22/26.
//

#include "../../include/model/shader_parser/ShaderParser.h"

#include "model/shader_parser/GLSLLexer.h"

ShaderParser::ShaderParser(const std::string& fileContent, const std::string& path, std::ostringstream& warnings, const std::vector<DefineField>& defines):
        Parser(new GLSLLexer(fileContent, path), path, warnings),
        mDefineFields(defines)
{
    // Expected returned code size
    mCode.reserve(fileContent.size() + defines.size()*64);
}

void ShaderParser::parseImpl()
{
    next();
    bool needsSpace = false;
    auto needsToBeSpaced = [](const LexerTokenType &type)
    {
        return type == IDENTIFIER || type == FLOAT || type == INT || type == GLSL_DIRECTIVE;
    };
    while (mCurrent.type != END)
    {
        if (needsToBeSpaced(mCurrent.type))
        {
            if (needsSpace)
                mCode += " ";
            needsSpace = true;
        }
        else
        {
            needsSpace = false;
        }
        if (mCurrent.type == NEWLINE)
        {
            mCode += '\n';
            next();
        }
        else if (mCurrent.type == GLSL_DIRECTIVE && mCurrent.identifier == "#version")
        {
            parseVersion();
        }
        else
        {
            mCode += mCurrent.identifier;
            next();
        }
    }
}

void ShaderParser::parseVersion()
{
    mCode += mCurrent.identifier;
    next();
    while (mCurrent.type != NEWLINE)
    {
        mCode += " ";
        mCode += mCurrent.identifier;
        next();
    }
    mCode += '\n';
    addDefines();
}

void ShaderParser::addDefines()
{
    int count = 0;
    for (auto [field, value]:mDefineFields)
    {
        mCode += "#define " + field + " " + value;
        if (count++ < mDefineFields.size() - 1)
            mCode += '\n';
    }
}
