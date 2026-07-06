//
// Created by jassoka on 7/4/26.
//

#include "Parser.h"

#include "model/MeshBuilder.h"
#include "util/file_funcs.hpp"

Parser::Parser(const std::string &file, Scene* scene):
    mLexer(Lexer(readFileToString(file))),
    mScene(scene),
    mDir(file.substr(0, file.find_last_of('/') + 1)),
    mMeshBuildFlags(new MeshBuildFlags{}),
    mMeshBuildData(new MeshBuildData{})
{}


void Parser::next()
{
    mCurrent=mLexer.next();
}

void Parser::throwError(const std::string &msg) const {
    throw ParserException(msg,mLexer.getLine(),mLexer.getLine());
}