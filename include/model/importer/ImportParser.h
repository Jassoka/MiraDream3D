//
// Created by jassoka on 7/22/26.
//

#ifndef MIRADREAM3D_IMPORTPARSER_H
#define MIRADREAM3D_IMPORTPARSER_H
#include "model/parser/Parser.h"


class ImportParser : public Parser
{
protected:
    ImportParser(const std::string &file, std::ostringstream &warningStream, SceneImport &sceneOutput);
    void executeParser() override;
    void virtual initFlags() = 0;
    glm::vec2 parseVec2() { return parseVec<2>(); }
    glm::vec3 parseVec3() { return parseVec<3>(); }

    MeshBuildFlags *mMeshBuildFlags;
    MeshBuildData *mMeshBuildData;
    /** @brief Contains this instance's imported scene elements */
    SceneImport &mSceneImport;
private:
    template <int dimension>
    glm::vec<dimension, float> parseVec();
};

#endif //MIRADREAM3D_IMPORTPARSER_H
