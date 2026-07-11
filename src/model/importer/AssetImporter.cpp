#include "model/Scene.h"
#include "model/Node.h"
#include "../../../include/model/importer/AssetImporter.h"

#include "model/importer/ObjParser.hpp"

void AssetImporter::importFile(const fileExtension ext, const std::string& path, Scene* scene, std::string& warnings)
{
    std::ostringstream warningStream;
    switch (ext)
    {
        case OBJ:
            ObjParser::parse(path,scene,warningStream);
        break;
    }
    warnings = warningStream.str();
}
