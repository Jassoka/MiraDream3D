#include "model/Scene.h"
#include "model/Node.h"
#include "model/AssetImporter.h"

#include "ObjParser.hpp"


void AssetImporter::loadObjFile(const std::string &path, Scene* scene) {
    ObjParser::parse(path,scene);
}