//
// Created by jassoka on 7/22/26.
//

#include "model/importer/ImportParser.h"
#include "model/MeshBuilder.h"

ImportParser::ImportParser(const std::string& file, std::ostringstream& warningStream, SceneImport& sceneOutput):
    Parser(file, warningStream),
    mSceneImport(sceneOutput),
    mMeshBuildFlags(new MeshBuildFlags{}),
    mMeshBuildData(new MeshBuildData{}) {}

void ImportParser::executeParser()
{
    initFlags();
    Parser::executeParser();
}

template <int dimension>
glm::vec<dimension, float> ImportParser::parseVec() {
    auto v = glm::vec<dimension, float>(0.);
    for (int i = 0; i < dimension; i++)
        v[i] = parseNumber();
    return v;
}

template glm::vec<2, float> ImportParser::parseVec<2>();
template glm::vec<3, float> ImportParser::parseVec<3>();