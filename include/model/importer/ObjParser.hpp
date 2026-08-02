//
// Created by Mathis Pean on 09/06/2026.
//

#ifndef MIRADREAM3D_OBJPARSING_HPP
#define MIRADREAM3D_OBJPARSING_HPP
#include <string>
#include <unordered_map>
#include <vector>

#include "ImportParser.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#define MAX_OBJ_NAME_SIZE 10

class Mesh;
class Node;
class Scene;
struct MeshBuildFlags;
struct MeshBuildData;

class ObjParser final: public ImportParser {
public:
    //friend class Parser;
    static void parse(const std::string &path, std::ostringstream& warnings, SceneImport &sceneOutput)
    {
        auto instance = ObjParser(path, warnings, sceneOutput);
        instance.executeParser();
    }
protected:
    explicit ObjParser(const std::string &path, std::ostringstream& warnings, SceneImport &sceneOutput);
    ~ObjParser() override;
    void initFlags() override;
    void parseImpl() override;
private:
    Node* mCurrentNode=nullptr;
    int32_t mCurrentMeshID = -1;
    int32_t mCurrentSubMeshID = -1;
    uint32_t mCurrentMeshOriginVId=0;
    bool mCurrentMeshHasUVCoords=true;
    uint8_t mCurrentSmoothGroup=0;

    std::unordered_map<uint8_t,uint8_t> mCurrentMeshSmoothGroupsMap;
    std::unordered_map<uint32_t,uint32_t> mCurrentMeshGeometricVerticesMap;
    std::vector<glm::vec3> mV;
    std::vector<glm::vec3> mVN;
    std::vector<glm::vec2> mVT;
    std::vector<uint32_t> mNoNormal;
    std::vector<uint32_t> mNoUv;

    void finishMesh();
    void parseV();
    void parseVN();
    void parseVT();
    void parseF();
    void parseO();
    void parseG();
    void parseS();
    void parseUsemtl();
    void parseMtllib();
    void parseL();

    void createMesh(const std::string &name);

};

#endif //MIRADREAM3D_OBJPARSING_HPP
