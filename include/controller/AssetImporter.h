#ifndef MIRADREAM3D_LOADFILE_H
#define MIRADREAM3D_LOADFILE_H
#include "model/Scene.h"

/**
 * @brief Class which implements static methods for assert importation within the software
 */
class AssetImporter
{
public:
    AssetImporter(Scene *scene);
    /**
     * @brief Loads a scene of any supported extension using Assimp library to the asset importer's scene
     * @param path Path of the source file
     * @return Success of import
     */
    bool loadAssimpScene(const std::string &path) const;
private:
    Scene *mScene;
};


#endif //MIRADREAM3D_LOADFILE_H
