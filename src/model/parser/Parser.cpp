//
// Created by jassoka on 7/4/26.
//

#include "model/parser/Parser.h"
#include "util/file_funcs.hpp"

Parser::Parser(const std::string& file, std::ostringstream& warningStream):
    mLexer(Lexer(readFileToString(file), file)),

    mDir(file.substr(0, file.find_last_of('/') + 1)),
    mWarningThrown(false),
    mWarnings(warningStream)
{}

void Parser::executeParser()
{
    parseImpl();
}

void Parser::next()
{
    mCurrent=mLexer.next();
}

void Parser::skipLine()
{
    while (mCurrent.type != NEWLINE) {
        next();
    }
}

void Parser::throwError(const std::string &msg, const std::string &detail) const {
    if (detail.empty())
        throw ParserException(msg, mLexer.getFilePath(),mLexer.getLine(),mLexer.getLine());
    throw ParserException(msg + ": " + detail,mLexer.getFilePath(),mLexer.getLine(),mLexer.getLine());
}

void Parser::throwWarning(const std::string& msg, const std::string &detail)
{
    mWarningThrown = true;
    if (detail.empty())
        mWarnings << parserMessageFormat(WARNING, "Parser", mLexer.getFilePath(), msg, mLexer.getLine(),mLexer.getLine()) << '\n';
    else
        mWarnings << parserMessageFormat(WARNING, "Parser", mLexer.getFilePath(), msg + ": " + detail, mLexer.getLine(),mLexer.getLine()) << '\n';
}


float Parser::parseNumber()
{
    float result;
    if (mCurrent.type==FLOAT)
        result = mCurrent.value.floatValue;
    else if (mCurrent.type==INT)
        result = static_cast<float>(mCurrent.value.intValue);
    else
        throwError(ParserMessages::ExpectedNumber);
    next();
    return result;
}

int Parser::parseInt()
{
    if (mCurrent.type==INT)
    {
        const int result = mCurrent.value.intValue;
        next();
        return result;
    }
    throwError(ParserMessages::ExpectedInt);
}

void Parser::expectEOL()
{
    if (mCurrent.type != NEWLINE && mCurrent.type != END )
        throwError(ParserMessages::ExpectedEOL);
    next();
}
void Parser::expectToken(const LexerTokenType tokenType, const std::string& msg)
{
    if (mCurrent.type != tokenType)
        throwError(msg);
    next();
}

std::string Parser::parseString()
{
    std::string name="";
    while (mCurrent.type != NEWLINE && mCurrent.type != END) {
        name += mCurrent.identifier;   // accumule tous les tokens
        next();
    }
    return name;
}