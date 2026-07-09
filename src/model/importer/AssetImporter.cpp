#include "model/Scene.h"
#include "model/Node.h"
#include "model/AssetImporter.h"

#include "ObjParser.hpp"

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
