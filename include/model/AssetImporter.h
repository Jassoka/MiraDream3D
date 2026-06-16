#ifndef MIRADREAM3D_ASSETIMPORTER_H
#define MIRADREAM3D_ASSETIMPORTER_H

class Scene;
/**
 * @brief Class which implements static methods for assert importation within the software
 */
class AssetImporter
{
public:
    /**
     * @brief Loads a scene of any supported extension using Assimp library
     * @param path Path of the source file
     * @param scene Destination scene to which is appended the imported scene
     * @return Success of import
     */
    static bool loadAssimpScene(const std::string &path, Scene *scene);
    static bool loadObjFile(const std::string &path, Scene* scene);
};

#endif //MIRADREAM3D_ASSETIMPORTER_H